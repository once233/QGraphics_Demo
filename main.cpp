
#include <QApplication>
#include <QBitmap>
#include <QDebug>
#include <QDesktopServices>
#include <QDrag>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QUrl>
#include <QtMath>
#include "math.h"

//加载图片
/*
 *那么安全和正确的方法应该是什么呢？答案是我们需要用QImage做一下预处理：

//correct and recommended way

QImage image;

image.load( ":/pics/earth.png" );

QPainter painter(this);

QPixmap pixmapToShow = QPixmap::fromImage( image.scaled(size(), Qt::KeepAspectRatio) );

painter.drawPixmap(0,0, pixmapToShow);

和QPixmap
不同，QImage是独立于硬件的，它可以同时被另一个线程访问。QImage是存储在客户端的，对QImage的使用是非常方便和安全的。
又由于 QImage 也是一种QPaintDevice，因此我们可以在另一个线程中对其进行绘制，而不需要在GUI
线程中处理，使用这一方式可以很大幅度提高UI响应速度。
因此当图片较大时，我们可以先通过QImage将图片加载进来，然后把图片缩放成需要的尺寸，最后转换成QPixmap 进行显示。
下图是显示效果(图片是按照earth.png的原始尺寸比例缩放后显示的)：

其中需要注意的是Qt::KeepAspectRatio的使用，默认参数是Qt::IgnoreAspectRatio，如果我们在程序中这么写：

QPixmap pixmapToShow = QPixmap::fromImage( image.scaled(size(), Qt::IgnoreAspectRatio) );

效果就是下面这个样子，earth.png被拉伸以充满整个屏幕：

直接使用QImage 显示

我们也可以直接使用QImage做显示，而不转换成QPixmap ，这要根据我们应用的具体需求来决定，如果需要的话我们可以这么写：

//correct, some times may be needed

QImage image;

image.load( ":/pics/earth.png" );

QPainter painter(this);

painter.drawImage(0,0, image);

 *
 *
 *
 */

/* RenderHint
 *
 * QPainter::TextAntialiasing //绘制的字体抗锯齿
 * setRenderHint(QPainter::Antialiasing) 绘图抗锯齿
 * setRenderHint(QPainter::Antialiasing, false); 关闭绘图抗锯齿
 *
 */

/* ViewportUpdateMode  刷新模式
 *
 * QGraphicsView::BoundingRectViewportUpdate   花最少时间定位重绘区域，但没有改的区域也要重绘
 * MinimalViewportUpdate   当内容改变时候QGraphicsView将会刷新尽可能小的视图点
 * FullViewportUpdate QGraphicsView花费更多的时间来计算要绘制的内容时，这种方法最快（例如，当很多小项目被重复更新时）
 * SmartViewportUpdate QGraphicsView将通过分析需要重新绘制的区域，尝试找到最佳的更新模式。
 * NoViewportUpdate QGraphicsView将永远不会更新其视口；用户需要控制所有更新
 * 适用于需要固定帧速率的场景，或者视口以其他方式在外部更新的场景
 */

//画线画文字
/* draw
 *
 * drawText 画文字
 * drawLine 画直线 提供起点、终点的坐标就可以了
 * drawEllipse 画椭圆（圆） 如果指定的矩形框是正方形，画出来就是圆形，否则就是椭圆
 * QPen 用于绘制轮廓线
 * QBrush 用于填充
 *
 * setRect(x,y,w,h)
 *
 *                 (0,0)
 *                   .----------------->x
 *                   |
 *                   |
 *                   |
 *                   y
 */

// QGraphicsItem
/* QGraphicsItem
 * QGraphicsItem是图元的基类，用户可以继承QGraphicsItem实现自定义的图元；
 *
 * QGraphicsEllipseItem 椭圆
 * QGraphicsLineItem 直线  setLine(x,y,tox,toy)
 * QGraphicsPathItem 路径
 * QGraphicsPixmapItem 图像
 * QGraphicsPolygonItem 多边形
 * QGraphicsRectItem 矩形
 *
 *
 * QGraphicsSimpleTextItem 简单文本
 * QGraphicsTextItem 文本浏览
 *
 * Qt::TextInteractionFlag
 * Qt::NoTextInteraction：不与文本交互。
 * Qt::TextSelectableByMouse：可以使用鼠标选择文本并使用上下文菜单或快捷键将文本复制到剪贴板。
 * Qt::TextSelectableByKeyboard：可以使用键盘上的光标键选择文本。显示文本光标。
 * Qt::LinksAccessibleByMouse：可以用鼠标突出显示和激活链接。
 * Qt::LinksAccessibleByKeyboard：链接可以使用 tab 聚焦并使用 enter 激活。
 * Qt::TextEditable：文本可编辑。
 * Qt::TextEditorInteraction：文本编辑器的默认设置。（TextSelectableByMouse | TextSelectableByKeyboard | TextEditable）
 * Qt::TextBrowserInteraction：QTextBrowser 的默认设置。（TextSelectableByMouse | LinksAccessibleByMouse |
 * LinksAccessibleByKeyboard）
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * setFlag(QGraphicsItem::ItemIsMovable, true);  //设置可拖动 或用mouseevent
 * void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
 * void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
 * void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
 *
 * 设置旋转
 * 通过向量的 点乘 ，计算角度。单位向量点乘的值，正好为角度的余弦。
 * 通过向量的 叉乘
 * 计算旋转的方向。叉乘的结果为与这两个向量垂直的向量，可以通过Z轴结果判断，如果结果为正表示顺时针，结果为负表示逆时针。
 *
 */

//拖动旋转
/*

void mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        m_transform = this->transform();

        // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();

        // 保存当前的一些信息
    m_pos = pos;
    m_pressedPos = scenePos;
    m_startPos = this->pos();
    return QGraphicsItem::mousePressEvent(event);
}

void UICanvasItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF loacalPos = event->pos();

        // 获取并设置为单位向量
    QVector2D startVec(m_pos.x() - 0, m_pos.y() - 0);
    startVec.normalize();
    QVector2D endVec(loacalPos.x() - 0, loacalPos.y() - 0);
    endVec.normalize();

    // 单位向量点乘，计算角度
    qreal dotValue = QVector2D::dotProduct(startVec, endVec);
    if (dotValue > 1.0)
        dotValue = 1.0;
    else if (dotValue < -1.0)
        dotValue = -1.0;

    dotValue = qAcos(dotValue);
    if (isnan(dotValue))
        dotValue = 0.0;

    // 获取角度
    qreal angle = dotValue * 1.0 / (PI / 180);

    // 向量叉乘获取方向
    QVector3D crossValue = QVector3D::crossProduct( \
        QVector3D(startVec, 1.0), \
        QVector3D(endVec, 1.0));

    if (crossValue.z() < 0)
        angle = -angle;
    m_rotate += angle;

    // 设置变化矩阵
    m_transform.rotate(m_rotate);
    this->setTransform(m_transform);

    m_pos = loacalPos;
    this->update();
}
*/

//弧度
/*sin(弧度)
 *
 *
 * l(弧长)/r(半径) = 弧度 完整的圆的弧度是2π
 * C(周长)/r      =2PI
 *2PI=360度，C=2*PI*R ,C/R=2*PI,xl
 *

 */

class myDropItem : public QGraphicsObject {
  QColor color = Qt::lightGray;

 public:
  myDropItem();
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

 protected:
  void dropEvent(QGraphicsSceneDragDropEvent *event) override;
  void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
  void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
};

myDropItem::myDropItem() { setAcceptDrops(true); }

QRectF myDropItem::boundingRect() const { return QRectF(-30, -30, 90, 90); }
void myDropItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(color);
  painter->drawEllipse(-28, -28, 85, 85);

  painter->drawText(0, 0, "into color");
}

void myDropItem::dropEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasColor()) {
    qDebug() << "dropEvent";
    event->setAccepted(true);
    color = qvariant_cast<QColor>(event->mimeData()->colorData());
    update();
  } else {
    qDebug() << "!dropEvent";
    event->setAccepted(false);
  }
}
void myDropItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
  Q_UNUSED(event);
  qDebug() << "dragLeaveEvent";
  color = Qt::gray;
  update();
}

void myDropItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (event->mimeData()->hasColor()) {
    qDebug() << "dragEnterEvent";
    event->setAccepted(true);
    color = qvariant_cast<QColor>(event->mimeData()->colorData()).lighter(130);
    update();
  } else {
    qDebug() << "!dragEnterEvent";
    event->setAccepted(false);
  }
}

class myEllipseItem : public QGraphicsItem {
  QColor color;

 public:
  myEllipseItem();

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

 protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

myEllipseItem::myEllipseItem()
    : color(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256)) {
  setCursor(Qt::OpenHandCursor);
  setToolTip(QString("RGB(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()));
  setAcceptedMouseButtons(Qt::LeftButton);
}

void myEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  painter->setPen(Qt::NoPen);
  painter->setBrush(Qt::darkGray);
  painter->drawEllipse(-12, -12, 30, 30);
  painter->setPen(QPen(Qt::black, 1));
  painter->setBrush(QBrush(color));
  painter->drawEllipse(-15, -15, 30, 30);
}
QRectF myEllipseItem::boundingRect() const { return QRectF(-15.5, -15.5, 34, 34); }

void myEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() <
      QApplication::startDragDistance()) {
    return;
  }
  qDebug() << "length:" << QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length()
           << "event->screenPos():" << event->screenPos()
           << "event->buttonDownScreenPos(Qt::LeftButton):" << event->buttonDownScreenPos(Qt::LeftButton);
  QDrag *drag = new QDrag(event->widget());
  QMimeData *mimedata = new QMimeData;

  drag->setMimeData(mimedata);

  mimedata->setColorData(color);
  mimedata->setText(QString("#%1%2%3")
                        .arg(color.red(), 2, 16, QLatin1Char('0'))
                        .arg(color.green(), 2, 16, QLatin1Char('0'))
                        .arg(color.blue(), 2, 16, QLatin1Char('0')));
  QPixmap pixmap(34, 34);

  pixmap.fill(Qt::white);
  QPainter painter(&pixmap);
  painter.translate(15, 15);
  painter.setRenderHint(QPainter::Antialiasing);
  paint(&painter, nullptr, nullptr);
  painter.end();
  pixmap.setMask(pixmap.createHeuristicMask());
  drag->setPixmap(pixmap);
  drag->setHotSpot(QPoint(15, 20));

  drag->exec();
  setCursor(Qt::OpenHandCursor);
}
void myEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event) { setCursor(Qt::ClosedHandCursor); }
void myEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) { setCursor(Qt::OpenHandCursor); }

class GraphicsView : public QGraphicsView {
 public slots:
  void zoomIn() { scale(1.2, 1.2); }
  void zoomOut() { scale(1 / 1.2, 1 / 1.2); }
  void rotateLeft() { rotate(-30); }
  void rotateRight() { rotate(30); }

 public:
 protected:
  void resizeEvent(QResizeEvent *event) override{};
};

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QGraphicsScene scene(QRect(-200, -200, 800, 800));

  GraphicsView view;
  view.setScene(&scene);
  view.setRenderHint(QPainter::Antialiasing, false);
  view.setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  view.setBackgroundBrush(QBrush(QColor(230, 200, 167)));
  view.setWindowTitle("draw demo");

  QPen pen;  // 定义一个画笔，设置画笔颜色和宽度
  pen.setColor(QColor(0, 160, 230));
  pen.setWidth(10);

  QGraphicsRectItem *itemRect = new QGraphicsRectItem();
  itemRect->setRect(-100, 0, 30, 30);
  itemRect->setPen(pen);
  itemRect->setBrush(QBrush(QColor(85, 200, 105)));
  itemRect->setFlag(QGraphicsItem::ItemIsMovable, true);
  scene.addItem(itemRect);

  QGraphicsLineItem *itemLine = new QGraphicsLineItem();
  itemLine->setLine(0, 0, -50, 50);
  itemLine->setPen(pen);
  itemLine->setFlag(QGraphicsItem::ItemIsMovable, true);
  scene.addItem(itemLine);

  QGraphicsPathItem *itemPath = new QGraphicsPathItem();
  QPainterPath path;

  //三角形
  //  path.moveTo(90, 50);
  //  for (int i = 1; i < 5; ++i) {
  //    path.lineTo(50 + 40 * cos(0.8 * i * M_PI), 50 + 40 * sin(0.8 * i * M_PI));
  //  }

  path.moveTo(0, -200);
  path.lineTo(0, 0);
  path.lineTo(100, 0);
  path.lineTo(100, -100);
  path.closeSubpath();
  itemPath->setPath(path);
  itemPath->setPen(pen);
  scene.addItem(itemPath);

  QGraphicsPolygonItem *itemPolygon = new QGraphicsPolygonItem;
  QPolygonF polygon;
  polygon << QPointF(-100, -150) << QPointF(-100, -250) << QPointF(-200, -250) << QPointF(-200, -150);
  itemPolygon->setPolygon(polygon);
  itemPolygon->setPen(pen);
  itemPolygon->setFlag(QGraphicsItem::ItemIsMovable, true);
  scene.addItem(itemPolygon);

  QGraphicsSimpleTextItem *itemSimpleText = new QGraphicsSimpleTextItem;
  itemSimpleText->setFont(QFont("华文琥珀", 17));
  itemSimpleText->setText("SimpleText");
  scene.addItem(itemSimpleText);
  itemSimpleText->setFlag(QGraphicsItem::ItemIsMovable);

  QGraphicsTextItem *itemText = new QGraphicsTextItem;
  itemText->setHtml("<a href=\"www.baidu.com\">网址</a>");
  itemText->setPos(QPointF(10, -90));
  scene.addItem(itemText);
  itemText->setTextInteractionFlags(Qt::TextBrowserInteraction);
  QObject::connect(itemText, &QGraphicsTextItem::linkActivated,
                   [](const QString &url) { QDesktopServices::openUrl(QUrl(url)); });

  QPushButton *m_zoomInBtn = new QPushButton("zoomIn");
  QPushButton *m_zoomOutBtn = new QPushButton("zoomOut");
  QPushButton *m_rotateLeftBtn = new QPushButton("rotateLeft");
  QPushButton *m_rotateRightBtn = new QPushButton("rotateRight");

  m_zoomInBtn->move(400, 10);
  m_zoomOutBtn->move(400, 50);
  m_rotateLeftBtn->move(400, 90);
  m_rotateRightBtn->move(400, 130);
  scene.addWidget(m_zoomInBtn);
  scene.addWidget(m_zoomOutBtn);
  scene.addWidget(m_rotateLeftBtn);
  scene.addWidget(m_rotateRightBtn);

  // 点击按钮实现视图的缩放和旋转
  QObject::connect(m_zoomInBtn, &QPushButton::clicked, [&]() { view.zoomIn(); });
  QObject::connect(m_zoomOutBtn, &QPushButton::clicked, [&]() { view.zoomOut(); });
  QObject::connect(m_rotateLeftBtn, &QPushButton::clicked, [&]() { view.rotateLeft(); });
  QObject::connect(m_rotateRightBtn, &QPushButton::clicked, [&]() { view.rotateRight(); });

  QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem;
  ellipseItem->setRect(200, 200, 200, 200);

  scene.addItem(ellipseItem);

  int r = 100;
  float rad = 2 * M_PI;
  for (int i = 0; i < 180; i++) {
    QGraphicsEllipseItem *ellipseItemb = new QGraphicsEllipseItem;

    ellipseItemb->setRect(100 + sin((rad * i) / 180) * r, 100 + cos((rad * i) / 180) * r, 30, 30);
    // ellipseItemb->setRect(::sin((i * 2 * M_PI) / 10.0) * 110, ::cos((i * 6.28) / 10.0) * 110, 30, 30);
    ellipseItemb->setBrush(QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256),
                                  QRandomGenerator::global()->bounded(256)));

    scene.addItem(ellipseItemb);
  }

  for (int i = 0; i < 10; i++) {
    myEllipseItem *myitem = new myEllipseItem;
    myitem->setPos(300 + sin(rad * i / 10) * r, 300 + cos(rad * i / 10) * r);
    scene.addItem(myitem);
  }

  myDropItem *dropItem = new myDropItem;

  dropItem->setPos(310, 310);
  scene.addItem(dropItem);
  view.show();
  return a.exec();
}
