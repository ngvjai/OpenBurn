#pragma once

#include <QGridLayout>
#include <QDialog>
#include <QGraphicsView>
#include <QPushButton>
#include <QComboBox>
#include <QToolButton>
#include <QGroupBox>

#include <QList>

#include "src/motor.hpp"
#include "src/settings.hpp"
#include "graindesigntypes.hpp"
#include "src/ui/graphics/graingraphicsitem.hpp"

class GrainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GrainDialog(PropellantList* propellants,
        OpenBurnGrain* seedValues = nullptr, 
        OpenBurnSettings* settings = nullptr,
        const GrainVector& grains = GrainVector(),
        QWidget* parent = nullptr
        );
    ~GrainDialog() = default;
protected:
    void resizeEvent(QResizeEvent* event) override;
	void closeEvent(QCloseEvent* event) override;
signals:
    void GrainAdded(const GrainPtr&);
    void GrainsEdited(const GrainVector& newGrains, const GrainVector& originalGrains);
	void DialogClosed();
private slots:
    void OnCancelButtonClicked();
    void OnApplyButtonClicked();
    void OnDesignUpdated();
    void OnGrainCoreTypeChanged(int);
private:
    enum class GrainType
    {
        BATES,
        MOON,
    };

    void SetupUI();
    void SeedValues(OpenBurnGrain* seed);
    void SetupGraphics();
    void UpdateGraphics();

    void UpdateDesignControls(GrainType type);
    void SetGrainValues();
    QGridLayout* m_layControls;
    QGroupBox* m_gbFrame;

    QGraphicsView *m_GraphicsView;
    QGraphicsScene *m_GraphicsScene;
    std::unique_ptr<GrainGraphicsItem> m_gfxGrain;
    std::unique_ptr<OpenBurnGrain> m_GrainForGfx; //gfx display, a clone of the seed grain
    QPushButton *m_btnApply, *m_btnCancel;

    std::unique_ptr<BaseGrainDesign> m_GrainDesign;

	QDoubleSpinBox *m_sbGrainDia, *m_sbGrainLen;
	QSpinBox *m_sbGrainInhibit;

	QComboBox *m_cbGrainType;
	QComboBox *m_cbPropellantType;
	LengthUnitsComboBox *m_unitsGrainLen, *m_unitsGrainDia;

	QToolButton *m_btntModifyPropellant;

    OpenBurnGrain* m_seedGrain;
	GrainVector m_grainsToEdit;
	//because the grains are pointers and we explicitly modify their values via the dialog, 
	//we store a copy of the original grains for the undo action later on
	GrainVector m_OriginalGrains;
    PropellantList* m_Propellants;
    OpenBurnSettings* m_GlobalSettings;
    bool m_isNewGrainWindow;
};
