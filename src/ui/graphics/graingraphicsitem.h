#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtCore>
#include <QPainter>
#include "src/grain.h"

//this class represents the 2D cross sectional view of a grain
class GrainGraphicsItem : public QGraphicsObject
{
public:
    GrainGraphicsItem(OpenBurnGrain* grain, int scale_factor, bool crossSection = true, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    QColor m_color;
    bool m_isCrossSectionView; //
    double grainX, grainY;
    int m_scaleFactor;
    OpenBurnGrain* m_grain;
};
