#ifndef WIZSERVICE_SYNC_H
#define WIZSERVICE_SYNC_H

#include "../share/wizSyncableDatabase.h"

bool WizSyncDatabase(IWizKMSyncEvents* pEvents,
                     IWizSyncableDatabase* pDatabase,
                     const WIZUSERINFO& info,
                     bool bBackground);

bool WizUploadDatabase(IWizKMSyncEvents* pEvents,
                       IWizSyncableDatabase* pDatabase,
                       const WIZUSERINFOBASE& info,
                       bool bGroup);

bool WizSyncDatabaseOnly(IWizKMSyncEvents* pEvents,
                         IWizSyncableDatabase* pDatabase,
                         const WIZUSERINFOBASE& info,
                         bool bGroup);


#endif // WIZSERVICE_SYNC_H
