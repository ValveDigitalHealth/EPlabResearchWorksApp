object Histogram_Form: THistogram_Form
  Left = 0
  Top = 0
  Caption = 'Histogram'
  ClientHeight = 613
  ClientWidth = 985
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Chart1: TChart
    Left = 8
    Top = 8
    Width = 761
    Height = 545
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    BottomAxis.LabelsFormat.Font.Height = -16
    BottomAxis.Title.Caption = 'unit'
    BottomAxis.Title.Font.Height = -16
    LeftAxis.LabelsFormat.Font.Height = -16
    LeftAxis.Title.Caption = 'unit'
    LeftAxis.Title.Font.Height = -16
    View3D = False
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TBarSeries
      Marks.Visible = False
      BarWidthPercent = 100
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object Close_Button: TButton
    Left = 854
    Top = 564
    Width = 123
    Height = 41
    Caption = 'CLOSE'
    TabOrder = 1
    OnClick = Close_ButtonClick
  end
  object RichEdit1: TRichEdit
    Left = 775
    Top = 8
    Width = 202
    Height = 545
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'RichEdit1')
    ParentFont = False
    TabOrder = 2
    Zoom = 100
  end
end
