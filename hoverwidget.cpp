#include "hoverwidget.h"
#include "qpainter.h"
#include "qpropertyanimation.h"

// Construtor padrão (para Qt Designer)
HoverWidget::HoverWidget(QWidget *parent)
    : QPushButton(parent),
    m_bgColor(Qt::white),
    m_normalColor(Qt::white),
    m_hoverColor(Qt::lightGray),
    m_textColor(Qt::black) {
    setAttribute(Qt::WA_Hover, true);
}

// Construtor completo (opcional, útil se quiser criar manualmente)
HoverWidget::HoverWidget(const QColor &normalColor,
                         const QColor &hoverColor,
                         QWidget *parent)
    : QPushButton(parent),
    m_bgColor(normalColor),
    m_normalColor(normalColor),
    m_hoverColor(hoverColor) {
    setAttribute(Qt::WA_Hover, true);
}

QColor HoverWidget::bgColor() const {
    return m_bgColor;
}

void HoverWidget::setBgColor(const QColor &color) {
    m_bgColor = color;
    update();
}

void HoverWidget::enterEvent(QEnterEvent *) {
    animateColor(m_bgColor, m_hoverColor);
}

void HoverWidget::leaveEvent(QEvent *) {
    animateColor(m_bgColor, m_normalColor);
}

void HoverWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fundo arredondado
    painter.setBrush(m_bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 12, 12);

    // Texto centralizado
    painter.setPen(m_textColor); // ou a cor que quiser
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void HoverWidget::animateColor(QColor from, QColor to) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "bgColor");    animation->setDuration(300);
    animation->setStartValue(from);
    animation->setEndValue(to);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// Método para definir cores dinamicamente
void HoverWidget::setColors(const QColor &normal, const QColor &hover) {
    m_normalColor = normal;
    m_hoverColor = hover;
    m_bgColor = normal;
    update();
}

void HoverWidget::setTextColor(const QColor &color) {
    m_textColor = color;
    update();
}

void HoverWidget::setConfigs(QString text,const QColor &color, const QColor &normal, const QColor &hover, QString config)
{
    this->setColors(normal, hover);
    this->setText(text);
    this->setTextColor(color);

    if(config=="calls"){
        this->setStyleSheet
            (R"(
            font-size: 30pt;
            font-weight: bold;
            min-width: 200px;
            min-height: 150px;
            max-width: 500px;
            max-height: 325px;
        )");
    }
    if(config=="admin"){
        this->setStyleSheet
            (R"(
            font-size: 20pt;
            font-weight: bold;
            min-width: 100px;
            min-height: 50px;
            max-width: 500px;
            max-height: 100px;
        )");
    }
    if(config=="defaultBtn"){
        this->setStyleSheet
            (R"(
            font-size: 20pt;
            font-weight: bold;
            min-width: 50px;
            min-height: 25px;
            max-width: 500px;
            max-height: 250px;
        )");
    }
    if(config=="loginBtn"){
        this->setStyleSheet
            (R"(
            font-size: 20pt;
            font-weight: bold;
            padding: 0 10px;
            min-width: 50px;
            min-height: 25px;
            max-width: 500px;
            max-height: 250px;
        )");
    }
    if(config=="showPassBtn"){
        this->setStyleSheet
            (R"(
            font-size: 15pt;
            font-weight: bold;
            min-width: 5px;
            min-height: 5px;
            max-width: 60px;
            max-height: 60px;
        )");
    }
    update();
}
