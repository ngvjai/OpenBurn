#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include "src/motorsim.h"
#include "src/settings.h"
#include "src/ui/widgets/qcustomplot.h"
#include "src/ui/dialogs/simsettingsdialog.h"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~SimulationTab();
public slots:
    void UpdateSimulation();
    void UpdateResults();
private slots:
    void RunSimButton_Clicked();
    void SimSettingsButton_Clicked();
    void SLOT_DesignReady();
private:
    void SetupUI();

    QLabel* m_maxPressureLabel, *m_BurnTimeLabel, *m_totalImpulseLabel, *m_motorDesignationLabel;
    QCustomPlot* m_Plotter;
    QPushButton* m_RunSimulationButton, *m_SimSettingsButton;
    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
    MotorSimSettings* m_SimSettings;
    SimSettingsDialog* m_SimSettingsDialog;
    OpenBurnSettings* m_GlobalSettings;
};
