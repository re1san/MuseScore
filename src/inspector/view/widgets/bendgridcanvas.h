/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MU_INSPECTOR_BENDGRIDCANVAS_H
#define MU_INSPECTOR_BENDGRIDCANVAS_H

#include <optional>

#include <QPainter>

#include "async/asyncable.h"

#include "modularity/ioc.h"
#include "ui/iuiconfiguration.h"

#include "types/bendtypes.h"

#include "uicomponents/view/quickpaintedview.h"

namespace mu::inspector {
class BendGridCanvas : public uicomponents::QuickPaintedView, public async::Asyncable
{
    Q_OBJECT

    INJECT(mu::ui::IUiConfiguration, uiConfig)

    Q_PROPERTY(QVariant pointList READ pointList WRITE setPointList NOTIFY pointListChanged)

    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount NOTIFY columnCountChanged)
    Q_PROPERTY(int rowSpacing READ rowSpacing WRITE setRowSpacing NOTIFY rowSpacingChanged)
    Q_PROPERTY(int columnSpacing READ columnSpacing WRITE setColumnSpacing NOTIFY columnSpacingChanged)
    Q_PROPERTY(bool shouldShowNegativeRows READ shouldShowNegativeRows WRITE setShouldShowNegativeRows NOTIFY shouldShowNegativeRowsChanged)

public:
    explicit BendGridCanvas(QQuickItem* parent = nullptr);

    QVariant pointList() const;

    int rowCount() const;
    int columnCount() const;
    int rowSpacing() const;
    int columnSpacing() const;

    bool shouldShowNegativeRows() const;

public slots:
    void setPointList(QVariant pointList);

    void setRowCount(int rowCount);
    void setColumnCount(int columnCount);
    void setRowSpacing(int rowSpacing);
    void setColumnSpacing(int columnSpacing);

    void setShouldShowNegativeRows(bool shouldShowNegativeRows);

signals:
    void canvasChanged();

    void pointListChanged(QVariant pointList);

    void rowCountChanged(int rowCount);
    void columnCountChanged(int columnCount);
    void rowSpacingChanged(int rowSpacing);
    void columnSpacingChanged(int columnSpacing);

    void shouldShowNegativeRowsChanged(bool shouldShowNegativeRows);

private:
    void paint(QPainter* painter) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void hoverEnterEvent(QHoverEvent* event) override;
    void hoverMoveEvent(QHoverEvent* event) override;
    void hoverLeaveEvent(QHoverEvent* event) override;

    QRectF frameRect() const;
    qreal columnWidth(const QRectF& frameRect) const;
    qreal rowHeight(const QRectF& frameRect) const;

    std::pair<int, int> frameCoord(const QRectF& frameRect, int x, int y) const;

    void drawBackground(QPainter* painter, const QRectF& frameRect);
    void drawCurve(QPainter* painter, const QRectF& frameRect);

    std::optional<int> pointIndex(const CurvePoint& pitch, bool movable = true) const;
    CurvePoint point(const QRectF& frameRect, int frameX, int frameY) const;

    CurvePoints m_points;

    /// The number of rows and columns.
    /// This is in fact the number of lines that are to be drawn.
    int m_rows    = 0;
    int m_columns = 0;
    /// the interval between darker lines.
    /// In the case of rows, since they represent the pitch, the primary lines represent semi-tones.
    /// Moving from a primary line to another means changing the pitch of a semitone.
    int m_primaryColumnsInterval = 0;
    int m_primaryRowsInterval    = 0;

    /// Show negative pitch values. Happens in tremoloBarCanvas.
    bool m_showNegativeRows = false;

    std::optional<int> m_hoverPointIndex;
    std::optional<int> m_currentPointIndex;

    bool m_canvasWasChanged = false;
};
}

#endif // MU_INSPECTOR_BENDGRIDCANVAS_H