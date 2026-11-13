import QtQuick

Canvas {
    id: canvas

    property var values: []
    property color lineColor: "#3daee9"
    property real maxValue: 0

    onValuesChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.reset()

        var w = width
        var h = height
        if (values.length < 2 || w <= 0 || h <= 0) {
            return
        }

        var max = maxValue > 0 ? maxValue : 1
        if (maxValue <= 0) {
            for (var m = 0; m < values.length; ++m) {
                max = Math.max(max, values[m])
            }
            max *= 1.15
        }

        var n = values.length
        var stepX = w / (n - 1)
        function px(i) { return i * stepX }
        function py(v) { return h - Math.max(0, Math.min(1, v / max)) * (h - 2) - 1 }

        var gradient = ctx.createLinearGradient(0, 0, 0, h)
        gradient.addColorStop(0, Qt.rgba(lineColor.r, lineColor.g, lineColor.b, 0.32))
        gradient.addColorStop(1, Qt.rgba(lineColor.r, lineColor.g, lineColor.b, 0.02))

        ctx.beginPath()
        ctx.moveTo(0, h)
        for (var i = 0; i < n; ++i) {
            ctx.lineTo(px(i), py(values[i]))
        }
        ctx.lineTo(w, h)
        ctx.closePath()
        ctx.fillStyle = gradient
        ctx.fill()

        ctx.beginPath()
        for (var j = 0; j < n; ++j) {
            if (j === 0) {
                ctx.moveTo(px(j), py(values[j]))
            } else {
                ctx.lineTo(px(j), py(values[j]))
            }
        }
        ctx.strokeStyle = lineColor
        ctx.lineWidth = 1.6
        ctx.lineJoin = "round"
        ctx.stroke()
    }
}
