# QGraphics_Demo

//弧度
/*sin(弧度)
 *
 *
 * l(弧长)/r(半径) = 弧度 完整的圆的弧度是2π
 * C(周长)/r      =2PI
 *2PI=360度，C=2*PI*R ,C/R=2*PI,xl
 *

 */
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





QGraphics各种item的例子
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
