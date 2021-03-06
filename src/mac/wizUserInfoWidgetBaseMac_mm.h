#ifndef WIZUSERINFOWIDGETBASEMAC_H
#define WIZUSERINFOWIDGETBASEMAC_H


#include <QMacCocoaViewContainer>
#include <QIcon>

class QMenu;
#ifdef Q_OS_OSX
Q_FORWARD_DECLARE_OBJC_CLASS(NSMenu);
#endif

class CWizUserInfoWidgetBaseMac : public QMacCocoaViewContainer
{
    Q_OBJECT

public:
    explicit CWizUserInfoWidgetBaseMac(QWidget *parent = 0);

protected:
    QString m_text;
    QMenu* m_menuPopup;
    //
    int m_textWidth;
    int m_textHeight;
    //
    QPixmap m_circleAvatar;

    void setMenu(QMenu* menu) { m_menuPopup = menu; }
    //
    void calTextSize();
    //
    void updateUI();
public:

    QString text() const;
    void setText(QString val);

    virtual QPixmap getCircleAvatar(int width, int height);
    //
    NSMenu* getNSMewnu();
    //
    virtual QString userId() { return QString(); }
    virtual QPixmap getAvatar() { return QPixmap(); }
    virtual QIcon getArrow() { return QIcon(); }
    virtual int textWidth() const;
    virtual int textHeight() const;
};



#endif // WIZUSERINFOWIDGETBASEMAC_H
