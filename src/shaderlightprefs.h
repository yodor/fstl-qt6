#ifndef SHADERLIGHTPREFS_H
#define SHADERLIGHTPREFS_H

#include <QDialog>

class Canvas;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;          // ← Added (good practice)
class QResizeEvent;         // ← Added
class QMoveEvent;           // ← Added

class ShaderLightPrefs : public QDialog
{
    Q_OBJECT

public:
    explicit ShaderLightPrefs(QWidget* parent, Canvas* canvas);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void moveEvent(QMoveEvent* event) override;

private slots:
    void buttonAmbientColorClicked();
    void editAmbientFactorFinished();
    void resetAmbientColorClicked();

    void buttonDirectiveColorClicked();
    void editDirectiveFactorFinished();
    void resetDirectiveColorClicked();

    void comboDirectionsChanged(int index);
    void resetDirection();

    void okButtonClicked();

private:
    Canvas* canvas;
    QPushButton* buttonAmbientColor;
    QLineEdit* editAmbientFactor;
    QPushButton* buttonDirectiveColor;
    QLineEdit* editDirectiveFactor;
    QComboBox* comboDirections;

    static const QString PREFS_GEOM;
};

#endif // SHADERLIGHTPREFS_H
