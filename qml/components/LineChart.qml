import QtQuick
import org.kde.kirigami as Kirigami

Canvas {
    id: chart

    property var values: []
    property var values2: []
    property color color1: Kirigami.Theme.highlightColor
    property color color2: Kirigami.Theme.positiveTextColor
    property real maxValue: 0
    property bool showGrid: true

    onValuesChanged: requestPaint()
    onValues2Changed: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.reset()

        var w = width
        var h = height
        if (w <= 0 || h <= 0) {
            return
        }

        var max = maxValue > 0 ? maxValue : 1
        if (maxValue <= 0) {
            for (var m = 0; m < values.length; ++m) {
                max = Math.max(max, values[m])
            }
            for (var m2 = 0; m2 < values2.length; ++m2) {
                max = Math.max(max, values2[m2])
            }
            max *= 1.2
        }

        if (showGrid) {
            ctx.strokeStyle = Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g,
                                      Kirigami.Theme.textColor.b, 0.08)
            ctx.lineWidth = 1
            for (var g = 0; g <= 4; ++g) {
                var y = h * g / 4
                ctx.beginPath()
                ctx.moveTo(0, y)
                ctx.lineTo(w, y)
                ctx.stroke()
            }
        }

        function drawSeries(data, color, fill) {
            if (!data || data.length < 2) {
                return
            }
            var n = data.length
            var stepX = w / (n - 1)
            function px(i) { return i * stepX }
            function py(v) { return h - Math.max(0, Math.min(1, v / max)) * (h - 8) - 4 }

            if (fill) {
                var gradient = ctx.createLinearGradient(0, 0, 0, h)
                gradient.addColorStop(0, Qt.rgba(color.r, color.g, color.b, 0.22))
                gradient.addColorStop(1, Qt.rgba(color.r, color.g, color.b, 0.01))
                ctx.beginPath()
                ctx.moveTo(0, h)
                for (var i = 0; i < n; ++i) {
                    ctx.lineTo(px(i), py(data[i]))
                }
                ctx.lineTo(w, h)
                ctx.closePath()
                ctx.fillStyle = gradient
                ctx.fill()
            }

            ctx.beginPath()
            for (var j = 0; j < n; ++j) {
                if (j === 0) {
                    ctx.moveTo(px(j), py(data[j]))
                } else {
                    ctx.lineTo(px(j), py(data[j]))
                }
            }
            ctx.strokeStyle = color
            ctx.lineWidth = 1.8
            ctx.lineJoin = "round"
            ctx.stroke()
        }

        drawSeries(values, color1, true)
        drawSeries(values2, color2, false)
    }
}
