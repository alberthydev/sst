#ifndef HOVERWIDGET_H
#define HOVERWIDGET_H

#include <QPushButton>
#include <QColor>

class HoverWidget : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)

public:
    explicit HoverWidget(QWidget *parent = nullptr);

    HoverWidget(const QColor &normalColor,
                const QColor &hoverColor,
                QWidget *parent = nullptr);

    QColor bgColor() const;
    void setBgColor(const QColor &color);
    void setColors(const QColor &normal, const QColor &hover);
    void setTextColor(const QColor &color);
    void setConfigs(QString text,const QColor &color, const QColor &normal, const QColor &hover, QString config);

protected:
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
    void paintEvent(QPaintEvent *) override;

private:
    QColor m_bgColor;
    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_textColor;

    void animateColor(QColor from, QColor to);
};

#endif // HOVERWIDGET_H
