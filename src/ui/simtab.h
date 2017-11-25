#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QPropertyAnimation>
#include <QToolButton>

#include "src/motorsim.h"
#include "src/settings.h"
#include "src/thirdparty/qcustomplot/qcustomplot.h"
#include "src/ui/dialogs/simsettingsdialog.h"
#include "src/ui/graphics/motorgraphicsitem.h"

class SimulationTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimulationTab(OpenBurnMotor* design, MotorSim* sim, OpenBurnSettings* settings, QWidget* parent = nullptr);
    ~SimulationTab();

    void UpdateSimulation();
    void UpdateResults();
    void UpdateGraphics(OpenBurnMotor* motor = nullptr);
    void resizeEvent(QResizeEvent* event) override;
private slots:
    void OnRunSimButtonClicked();
    void OnSimSettingsButtonClicked();
    void OnPlayAnimationButtonClicked();
    void OnToBeginningButtonClicked();
    void OnAnimationFinished();
    void OnDesignReady();
    void OnDesignUpdated();

    void OnSimSettingsChanged();
    void OnMotorSliceChanged(int sliceIndex); //scrubber bar
private:
    void SetupUI();
    void SetGraphNames();
    void SetupPlotter();
    void UpdatePlotter();
    void UpdatePlotterLine();
    void UpdateCurrentChamber(int currentSlice);

    //gfx
    MotorGraphicsItem* m_gfxMotor;
    QGraphicsView* m_MotorDisplayView;
    QGraphicsScene* m_MotorDisplayScene;

    QSlider* m_sldBurnTimeScrubBar;
    QPropertyAnimation *m_animation;
    QLabel* m_lblMaxpressure, *m_lblBurnTime, *m_lblTotalImpulse, *m_lblMotorDesignation, *m_lblCoreMassFlux, *m_lblIsp;
    QLabel* m_lblCurrentPressure, *m_lblCurrentCoreMassFlux, *m_lblCurrentThrust, *m_lblCurrentIsp;
    QPushButton* m_btnRunSim, *m_btnSimSettings, *m_btnPlayAnimation;
    QToolButton* m_btntToBeginning;

    QCustomPlot* m_Plotter;
    QCPItemLine* m_lineGraphTime;

    OpenBurnMotor* m_Motor;
    MotorSim* m_Simulator;
    MotorSimSettings* m_SimSettings;
    OpenBurnSettings* m_GlobalSettings;
    SimSettingsDialog* m_SimSettingsDialog;
};
