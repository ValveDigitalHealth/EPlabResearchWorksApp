object BarPlot_Form: TBarPlot_Form
  Left = 0
  Top = 0
  ClientHeight = 551
  ClientWidth = 1112
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Top_Label: TLabel
    Left = 16
    Top = 8
    Width = 95
    Height = 25
    Caption = 'Top_Label'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Chart1: TChart
    Left = 8
    Top = 39
    Width = 1096
    Height = 498
    Title.Font.Color = clBlack
    Title.Font.Height = -19
    Title.Text.Strings = (
      'title')
    BottomAxis.Increment = 1.000000000000000000
    View3D = False
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TBarSeries
      Marks.Emboss.Clip = True
      Marks.Shadow.Clip = True
      Marks.Margins.Left = 10
      Marks.Clip = True
      Marks.SoftClip = True
      BarWidthPercent = 80
      Sides = 36
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object MainMenu1: TMainMenu
    Left = 728
    Top = 8
    object File1: TMenuItem
      Caption = 'File'
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
  end
end
