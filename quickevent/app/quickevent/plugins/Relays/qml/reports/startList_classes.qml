import qf.qmlreports 1.0
import shared.qml.reports 1.0
//import "qrc:/quickevent/core/js/ogtime.js" as OGTime

Report {
	id: root
	objectName: "root"

	property string reportTitle: qsTr("Start list by classes")
	property bool isBreakAfterEachClass: false
	property bool isColumnBreak: false
	property bool isPrintStartNumbers: false
	property var options

	//debugLevel: 1
	styleSheet: StyleSheet {
		objectName: "portraitStyleSheet"
		basedOn: ReportStyleCommon { id: myStyle }
		colors: [
		]
		pens: [
			Pen {name: "red1dot"
				basedOn: "black1"
				color: Color {def:"red"}
				style: Pen.DotLine
			},
			Pen {
				id: pen_black1
				basedOn: "black1"
			}
		]
	}
	textStyle: myStyle.textStyleDefault

	width: root.options.pageWidth? root.options.pageWidth: 210
	height: root.options.pageHeight? root.options.pageHeight: 297
	hinset: root.options.horizontalMargin? root.options.horizontalMargin: 10
	vinset: root.options.verticalMargin? root.options.verticalMargin: 5
	Frame {
		width: "%"
		height: "%"
		layout: Frame.LayoutStacked
		QuickEventHeaderFooter {
			reportTitle: root.reportTitle
		}
		Frame {
			width: "%"
			height: "%"
			columns: root.options.columns
			vinset: 10
			Band {
				id: band
				objectName: "band"
				width: "%"
				height: "%"
				QuickEventReportHeader {
					dataBand: band
					reportTitle: root.reportTitle
				}
				Space { height: 5 }
				Detail {
					id: detail
					objectName: "detail"
					width: "%"
					//keepAll: true
					layout: Frame.LayoutVertical
					function dataFn(field_name) {return function() {return rowData(field_name);}}
					Break {
						breakType: root.isColumnBreak? Break.Column: Break.Page;
						visible: root.isBreakAfterEachClass;
						skipFirst: true
					}
					Space {height: 2}
					Frame {
						width: "%"
						vinset: 1
						layout: Frame.LayoutHorizontal
						fill: Brush {color: Color {def: "khaki"} }
						Cell {
							width: "%"
							textFn: detail.dataFn("classes.name");
							textStyle: myStyle.textStyleBold
						}
						//Cell {
						//	textFn: function() { return qsTr("length: ") + detail.rowData("courses.length");}
						//}
					}
					//expandChildFrames: true
					Band {
						id: relayBand
						width: "%"
						//objectName: "relayBand"
						//keepFirst: 3
						//keepWithPrev: true
						htmlExportAsTable: true
						Detail {
							id: relayDetail
							//objectName: "runnersDetail"
							width: "%"
							layout: Frame.LayoutVertical
							function dataFn(field_name) {return function() {return rowData(field_name);}}
							Space {height: 2}
							Frame {
								width: "%"
								layout: Frame.LayoutHorizontal
								bottomBorder: Pen { basedOn: "black1" }
								//fill: Brush {color: Color {def: "khaki"} }
								textStyle: myStyle.textStyleBold
								Cell {
									width: "%"
									textFn: function() {
										return relayDetail.dataFn("relays.number")()
												+ ' ' + relayDetail.dataFn("relayName")()
												+ ' ' + relayDetail.dataFn("clubs.name")();
									}
								}
							}
							Band {
								id: lapsBand
								objectName: "lapsBand"
								width: "%"
								keepFirst: 10
								keepWithPrev: true
								htmlExportAsTable: true
								Detail {
									id: lapsDetail
									//objectName: "runnersDetail"
									width: "%"
									layout: Frame.LayoutHorizontal
									function dataFn(field_name) {return function() {return rowData(field_name);}}
									Cell {
										width: 15
										halign: Frame.AlignRight
										textFn: lapsDetail.dataFn("leg");
									}
									Cell {
										width: "%"
										textFn: lapsDetail.dataFn("competitorName");
									}
									Para {
										width: 18
										textFn: lapsDetail.dataFn("registration");
									}
									Cell {
										width: 18
										halign: Frame.AlignRight
										textFn: lapsDetail.dataFn("runs.siId");
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


