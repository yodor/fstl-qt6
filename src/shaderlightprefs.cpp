#include "shaderlightprefs.h"
#include "canvas.h"

#include <QApplication>
#include <QColorDialog>
#include <QDoubleValidator>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QComboBox>

const QString ShaderLightPrefs::PREFS_GEOM = "shaderPrefsGeometry";

ShaderLightPrefs::ShaderLightPrefs(QWidget* parent, Canvas* _canvas)
    : QDialog(parent)
    , canvas(_canvas)
{
    setWindowTitle("Shader Preferences");

    QVBoxLayout* prefsLayout = new QVBoxLayout(this);

    // Title
    QLabel* title = new QLabel("Shader preferences");
    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);           // Still acceptable, but see note below
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);
    prefsLayout->addWidget(title);

    // Middle section with grid layout
    QWidget* middleWidget = new QWidget;
    QGridLayout* middleLayout = new QGridLayout(middleWidget);
    prefsLayout->addWidget(middleWidget);

    // Labels
    middleLayout->addWidget(new QLabel("Ambient Color"), 0, 0);
    middleLayout->addWidget(new QLabel("Directive Color"), 1, 0);
    middleLayout->addWidget(new QLabel("Direction"), 2, 0);

    // Ambient Color
    QPixmap dummy(20, 20);
    dummy.fill(canvas->getAmbientColor());

    buttonAmbientColor = new QPushButton;
    buttonAmbientColor->setIcon(QIcon(dummy));
    buttonAmbientColor->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(buttonAmbientColor, 0, 1);

    editAmbientFactor = new QLineEdit;
    editAmbientFactor->setValidator(new QDoubleValidator(0.0, 10.0, 3, this));
    editAmbientFactor->setText(QString::number(canvas->getAmbientFactor(), 'f', 2));
    middleLayout->addWidget(editAmbientFactor, 0, 2);

    QPushButton* buttonResetAmbient = new QPushButton("Reset");
    buttonResetAmbient->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(buttonResetAmbient, 0, 3);

    // Directive Color
    dummy.fill(canvas->getDirectiveColor());

    buttonDirectiveColor = new QPushButton;
    buttonDirectiveColor->setIcon(QIcon(dummy));
    buttonDirectiveColor->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(buttonDirectiveColor, 1, 1);

    editDirectiveFactor = new QLineEdit;
    editDirectiveFactor->setValidator(new QDoubleValidator(0.0, 10.0, 3, this));
    editDirectiveFactor->setText(QString::number(canvas->getDirectiveFactor(), 'f', 2));
    middleLayout->addWidget(editDirectiveFactor, 1, 2);

    QPushButton* buttonResetDirective = new QPushButton("Reset");
    buttonResetDirective->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(buttonResetDirective, 1, 3);

    // Direction combo box
    comboDirections = new QComboBox;
    comboDirections->addItems(canvas->getNameDir());
    comboDirections->setCurrentIndex(canvas->getCurrentLightDirection());
    middleLayout->addWidget(comboDirections, 2, 1, 1, 2);

    QPushButton* buttonResetDirection = new QPushButton("Reset");
    buttonResetDirection->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(buttonResetDirection, 2, 3);

    // OK button row
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    QPushButton* okButton = new QPushButton("OK");
    okButton->setFocusPolicy(Qt::NoFocus);
    buttonLayout->addWidget(okButton);
    prefsLayout->addLayout(buttonLayout);

    // Modern signal/slot connections (recommended for Qt 6)
    connect(buttonAmbientColor,   &QPushButton::clicked, this, &ShaderLightPrefs::buttonAmbientColorClicked);
    connect(editAmbientFactor,    &QLineEdit::editingFinished, this, &ShaderLightPrefs::editAmbientFactorFinished);
    connect(buttonResetAmbient,   &QPushButton::clicked, this, &ShaderLightPrefs::resetAmbientColorClicked);

    connect(buttonDirectiveColor, &QPushButton::clicked, this, &ShaderLightPrefs::buttonDirectiveColorClicked);
    connect(editDirectiveFactor,  &QLineEdit::editingFinished, this, &ShaderLightPrefs::editDirectiveFactorFinished);
    connect(buttonResetDirective, &QPushButton::clicked, this, &ShaderLightPrefs::resetDirectiveColorClicked);

    connect(comboDirections,      &QComboBox::currentIndexChanged, this, &ShaderLightPrefs::comboDirectionsChanged);
    connect(buttonResetDirection, &QPushButton::clicked, this, &ShaderLightPrefs::resetDirection);
    connect(okButton,             &QPushButton::clicked, this, &ShaderLightPrefs::okButtonClicked);

    // Restore previous geometry
    QSettings settings;
    if (!settings.value(PREFS_GEOM).isNull()) {
        restoreGeometry(settings.value(PREFS_GEOM).toByteArray());
    }
}

// ===================================================================
// Slot implementations
// ===================================================================

void ShaderLightPrefs::buttonAmbientColorClicked()
{
    QColor newColor = QColorDialog::getColor(
        canvas->getAmbientColor(), this, "Choose Ambient Color");

    if (newColor.isValid()) {
        canvas->setAmbientColor(newColor);

        QPixmap dummy(20, 20);
        dummy.fill(canvas->getAmbientColor());
        buttonAmbientColor->setIcon(QIcon(dummy));

        canvas->update();
    }
}

void ShaderLightPrefs::editAmbientFactorFinished()
{
    canvas->setAmbientFactor(editAmbientFactor->text().toDouble());
    canvas->update();
}

void ShaderLightPrefs::resetAmbientColorClicked()
{
    canvas->resetAmbientColor();

    QPixmap dummy(20, 20);
    dummy.fill(canvas->getAmbientColor());
    buttonAmbientColor->setIcon(QIcon(dummy));

    editAmbientFactor->setText(QString::number(canvas->getAmbientFactor(), 'f', 2));
    canvas->update();
}

void ShaderLightPrefs::buttonDirectiveColorClicked()
{
    QColor newColor = QColorDialog::getColor(
        canvas->getDirectiveColor(), this, "Choose Directive Color");

    if (newColor.isValid()) {
        canvas->setDirectiveColor(newColor);

        QPixmap dummy(20, 20);
        dummy.fill(canvas->getDirectiveColor());
        buttonDirectiveColor->setIcon(QIcon(dummy));

        canvas->update();
    }
}

void ShaderLightPrefs::editDirectiveFactorFinished()
{
    canvas->setDirectiveFactor(editDirectiveFactor->text().toDouble());
    canvas->update();
}

void ShaderLightPrefs::resetDirectiveColorClicked()
{
    canvas->resetDirectiveColor();

    QPixmap dummy(20, 20);
    dummy.fill(canvas->getDirectiveColor());
    buttonDirectiveColor->setIcon(QIcon(dummy));

    editDirectiveFactor->setText(QString::number(canvas->getDirectiveFactor(), 'f', 2));
    canvas->update();
}

void ShaderLightPrefs::comboDirectionsChanged(int index)
{
    canvas->setCurrentLightDirection(index);
    canvas->update();
}

void ShaderLightPrefs::resetDirection()
{
    canvas->resetCurrentLightDirection();
    comboDirections->setCurrentIndex(canvas->getCurrentLightDirection());
    canvas->update();
}

void ShaderLightPrefs::okButtonClicked()
{
    close();
}

void ShaderLightPrefs::resizeEvent(QResizeEvent* event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QDialog::resizeEvent(event);
}

void ShaderLightPrefs::moveEvent(QMoveEvent* event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QDialog::moveEvent(event);
}
