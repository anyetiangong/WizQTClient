#include "wizkmsync.h"

#include <QDebug>

#include "apientry.h"
#include "token.h"

#include "../share/wizDatabase.h"

using namespace WizService;


/* ---------------------------- CWizKMSyncThead ---------------------------- */
void CWizKMSyncEvents::OnSyncProgress(int pos)
{
    Q_UNUSED(pos);
}

HRESULT CWizKMSyncEvents::OnText(WizKMSyncProgressStatusType type, const QString& strStatus)
{
    Q_UNUSED(type);
    qDebug() << "[Sync]" << strStatus;

    Q_EMIT messageReady(strStatus);
    return 0;
}

void CWizKMSyncEvents::SetDatabaseCount(int count)
{
    qDebug() << "[Sync]SetDatabaseCount count = " << count;
}

void CWizKMSyncEvents::SetCurrentDatabase(int index)
{
    qDebug() << "[Sync]SetCurrentDatabase index = " << index;
}

void CWizKMSyncEvents::OnTrafficLimit(IWizSyncableDatabase* pDatabase)
{
}

void CWizKMSyncEvents::OnStorageLimit(IWizSyncableDatabase* pDatabase)
{

}

void CWizKMSyncEvents::OnUploadDocument(const QString& strDocumentGUID, bool bDone)
{
    qDebug() << "[Sync]SetCurrentDatabase guid: " << strDocumentGUID;
}

void CWizKMSyncEvents::OnBeginKb(const QString& strKbGUID)
{
    qDebug() << "[Sync]OnBeginKb kb_guid: " << strKbGUID;
}

void CWizKMSyncEvents::OnEndKb(const QString& strKbGUID)
{
    qDebug() << "[Sync]OnEndKb kb_guid: " << strKbGUID;
}


/* ---------------------------- CWizKMSyncThead ---------------------------- */

#define FULL_SYNC_INTERVAL 15*60

CWizKMSyncThread::CWizKMSyncThread(CWizDatabase& db, QObject* parent)
    : QThread(parent)
    , m_db(db)
    , m_pEvents(NULL)
    , m_tLastSyncAll(QDateTime::currentDateTime())
    , m_mutex(new QMutex())
{
}

void CWizKMSyncThread::run()
{
    doSync();
}

/* sync type:
 * Manual: user click the sync button, kbguid is not needed.
 * BackgroundFull: auto triggered for a default interval, 15 minutes currently, kbguid is not needed.
 * ChangeTriggered: when user modify notes/attachments, sync is triggered, kbguid should not be empty.
 */
void CWizKMSyncThread::startSync(SyncType type, const QString& kbguid)
{
    if (type == ChangeTriggered) {
        Q_ASSERT(!kbguid.isEmpty());
    }

    qDebug() << "[Sync]startSync, thread: " << QThread::currentThreadId();

    if (isRunning()) {
        qDebug() << "[Sync]syncing is started and request is schedued.";

        m_mutex->lock();
        m_squeue.push_back(QString::number(type) + ":" + kbguid);
        m_mutex->unlock();

        return;
    }

    m_syncType = type;
    m_syncKb = kbguid;

    trySync();
}

void CWizKMSyncThread::trySync()
{
    qDebug() << "[Sync]trySync, thread: " << QThread::currentThreadId();

    connect(Token::instance(), SIGNAL(tokenAcquired(QString)), SLOT(onTokenAcquired(QString)), Qt::QueuedConnection);
    Token::requestToken();
}

void CWizKMSyncThread::onTokenAcquired(const QString& strToken)
{
    qDebug() << "[Sync]token acquired, thread: " << QThread::currentThreadId();

    Token::instance()->disconnect(this);

    if (strToken.isEmpty()) {
        Q_EMIT syncFinished(Token::lastErrorCode(), Token::lastErrorMessage());
        return;
    }

    m_info = Token::info();

    start(QThread::IdlePriority);
}

void CWizKMSyncThread::doSync()
{
    qDebug() << "[Sync]syncing started, thread:" << QThread::currentThreadId();

    if (needSyncAll())
    {
        syncAll();
        m_tLastSyncAll = QDateTime::currentDateTime();

        m_mutex->lock();
        m_squeue.clear();
        m_mutex->unlock();
        return;
    }

    quickSync();
}

bool CWizKMSyncThread::needSyncAll()
{
    if (m_syncType == Manual)
        return true;

    int nElapsed = m_tLastSyncAll.secsTo(QDateTime::currentDateTime());
    if (nElapsed > FULL_SYNC_INTERVAL)
        return true;

    return false;
}

bool CWizKMSyncThread::syncAll()
{
    syncUserCert();

    m_pEvents = new CWizKMSyncEvents();
    connect(m_pEvents, SIGNAL(messageReady(const QString&)), SIGNAL(processLog(const QString&)));

    m_pEvents->SetLastErrorCode(0);

    m_info.strKbGUID = m_db.kbGUID();
    m_info.strDatabaseServer = ApiEntry::kUrlFromGuid(m_info.strToken, m_info.strKbGUID);
    bool bBackground = (m_syncType == Manual) ? false : true;
    ::WizSyncDatabase(m_pEvents, &m_db, m_info, bBackground);

    m_pEvents->deleteLater();
    Q_EMIT syncFinished(m_pEvents->GetLastErrorCode(), "");
    return true;
}

// FIXME: remove this to syncing flow
void CWizKMSyncThread::syncUserCert()
{
    QString strN, stre, strd, strHint;

    CWizKMAccountsServer serser(WizService::ApiEntry::syncUrl());
    if (serser.GetCert(m_db.GetUserId(), m_db.GetPassword(), strN, stre, strd, strHint)) {
        m_db.SetUserCert(strN, stre, strd, strHint);
    }
}

bool CWizKMSyncThread::quickSync()
{
    Q_ASSERT(!m_syncKb.isEmpty());

    m_pEvents = new CWizKMSyncEvents();
    m_pEvents->SetLastErrorCode(0);
    connect(m_pEvents, SIGNAL(messageReady(const QString&)), SIGNAL(processLog(const QString&)));

    m_info.strKbGUID = m_syncKb;
    m_info.strDatabaseServer = ApiEntry::kUrlFromGuid(m_info.strToken, m_info.strKbGUID);
    ::WizUploadDatabase(m_pEvents, &m_db, m_info, m_db.kbGUID() == m_syncKb ? false : true);

    m_pEvents->deleteLater();
    Q_EMIT syncFinished(m_pEvents->GetLastErrorCode(), "");
    return true;
}

void CWizKMSyncThread::stopSync()
{
    if (isRunning() && m_pEvents) {
        m_pEvents->SetStop(true);
    }
}
