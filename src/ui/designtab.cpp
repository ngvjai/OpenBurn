#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>

#include <QDebug>

#include "src/ui/designtab.h"

DesignTab::DesignTab(QWidget* parent)
    : QWidget(parent), m_grainDialog(nullptr), m_nozzleDialog(nullptr)
{   
    SetupUI();
    connect(m_newGrainButton, SIGNAL(clicked()), this, SLOT(NewGrainButton_Clicked()));
    connect(m_nozzleSettingsButton, SIGNAL(clicked()), this, SLOT(NozzleButton_Clicked()));
    connect(m_grainsDisplay, SIGNAL(SIG_GrainPositionUpdated(int, int)),
        this, SLOT(SLOT_GrainPositionUpdated(int, int)));
}
DesignTab::~DesignTab() 
{

}
void DesignTab::SetupUI()
{
    //controls 
    QGroupBox* frame_GrainDesign = new QGroupBox(tr("Grain Design"));
    QGroupBox* frame_Params = new QGroupBox(tr("Simulation Settings"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    setSizePolicy(sizePolicy);    

    m_grainsDisplay = new GrainTableWidget(this);
    m_grainsDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_grainsDisplay->setDragEnabled(true);
    m_grainsDisplay->setDragDropMode(QAbstractItemView::DragDrop);
    m_grainsDisplay->setAlternatingRowColors(true);
    m_grainsDisplay->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_grainsDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_grainsDisplay->setColumnCount(5); //propellant, len, core dia, dia, inhibited face

    QStringList tableHeader = (QStringList() << 
        tr("Length") << 
        tr("Diameter") << 
        tr("Core Diameter") << 
        tr("Propellant") << 
        tr("Inhibited Faces"));
    m_grainsDisplay->setHorizontalHeaderLabels(tableHeader);
    
    //grain buttons
    m_newGrainButton = new QPushButton(tr("New Grain"));
    QGridLayout* gLayout = new QGridLayout;    
    gLayout->addWidget(m_newGrainButton);
    frame_GrainDesign->setLayout(gLayout);

    //nozzle and sim settings
    m_nozzleSettingsButton = new QPushButton(tr("Edit Nozzle"));
    QVBoxLayout* vLayout_2 = new QVBoxLayout;
    vLayout_2->addWidget(m_nozzleSettingsButton);
    frame_Params->setLayout(vLayout_2);

    //master layout
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(m_grainsDisplay, 0, 0);
    layout->addWidget(frame_GrainDesign, 0, 1);
    layout->addWidget(frame_Params, 0, 2);
    m_grainCrossSection = new QGraphicsView;
    
    //takes up 1 row, and two columns
    layout->addWidget(m_grainCrossSection, 1, 0, 1, 3);
    setLayout(layout);
}

//this allows us to mark the objects as null when they are destroyed, allowing new ones to be made later on
//NOTE: they are only deleted if delete is called directly since they do NOT have attribute WA_DeleteOnClose.
//this is because I want to be able to re-open the same dialog with the same settings easily.
void DesignTab::SLOT_NozzDialogClosed()
{
    m_nozzleDialog = nullptr;
    qDebug() << "deleted nozzle dialog!";
}
void DesignTab::SLOT_GrainDialogClosed()
{
    m_grainDialog = nullptr;
    qDebug() << "deleted grain dialog!";
}

//Recieved from the grain dialog. Updates the grain table widget,
//and send sout a signal to main window that there was a new grain added
void DesignTab::SLOT_NewGrain(OpenBurnGrain *grain)
{
    int numItems = m_grainsDisplay->rowCount();
    m_grainsDisplay->setRowCount(numItems+1);
    m_grainsDisplay->setItem(numItems, 0, new QTableWidgetItem(QString::number(grain->GetLength())));
    m_grainsDisplay->setItem(numItems, 1, new QTableWidgetItem(QString::number(grain->GetDiameter())));
    m_grainsDisplay->setItem(numItems, 2, new QTableWidgetItem(QString::number(grain->GetCoreDiameter())));    
    m_grainsDisplay->setItem(numItems, 3, new QTableWidgetItem(grain->GetPropellantType().GetPropellantName()));
    m_grainsDisplay->setItem(numItems, 4, new QTableWidgetItem(QString::number(grain->GetInhibitedFaces())));
    
    emit SIG_NewGrain(grain);
}
//Recieved from the grain table widget. Update the sim!
void DesignTab::SLOT_GrainPositionUpdated(int oldPos, int newPos)
{
    //todo: update sim?
}
void DesignTab::SLOT_NozzleUpdated(OpenBurnNozzle* nozz)
{
    qDebug() << "Nozzle throat diameter: " << nozz->GetNozzleThroat();
}
void DesignTab::NewGrainButton_Clicked()
{
    if (!m_grainDialog) //only make one!!
    {
        m_grainDialog = new GrainDialog(nullptr, true);
        connect(m_grainDialog, SIGNAL(SIG_DIALOG_NewGrain(OpenBurnGrain*)), this, SLOT(SLOT_NewGrain(OpenBurnGrain*)));
        connect(m_grainDialog, SIGNAL(destroyed()), this, SLOT(SLOT_GrainDialogClosed()));
        
    }
    m_grainDialog->show();
    m_grainDialog->activateWindow();
    m_grainDialog->raise();
}
void DesignTab::NozzleButton_Clicked()
{
    if (!m_nozzleDialog) //only make one!!
    {
        m_nozzleDialog = new NozzleDialog(nullptr);
        connect(m_nozzleDialog, SIGNAL(SIG_NozzleChanged(OpenBurnNozzle*)), this, SLOT(SLOT_NozzleUpdated(OpenBurnNozzle*)));
        connect(m_nozzleDialog, SIGNAL(destroyed()), this, SLOT(SLOT_NozzDialogClosed()));
    }
    m_nozzleDialog->show();
    m_nozzleDialog->activateWindow();
    m_nozzleDialog->raise();
}