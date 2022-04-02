
#include <QApplication>
#include <QDesktopServices>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QUrl>
#include "math.h"
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
  m_zoomOutBtn->move(400, 30);
  m_rotateLeftBtn->move(400, 50);
  m_rotateRightBtn->move(400, 70);
  scene.addWidget(m_zoomInBtn);
  scene.addWidget(m_zoomOutBtn);
  scene.addWidget(m_rotateLeftBtn);
  scene.addWidget(m_rotateRightBtn);

  // 点击按钮实现视图的缩放和旋转
  QObject::connect(m_zoomInBtn, &QPushButton::clicked, [&]() { view.zoomIn(); });
  QObject::connect(m_zoomOutBtn, &QPushButton::clicked, [&]() { view.zoomOut(); });
  QObject::connect(m_rotateLeftBtn, &QPushButton::clicked, [&]() { view.rotateLeft(); });
  QObject::connect(m_rotateRightBtn, &QPushButton::clicked, [&]() { view.rotateRight(); });

  view.show();
  return a.exec();
}
