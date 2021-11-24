object PaintBoxForm: TPaintBoxForm
  Left = 0
  Top = 0
  Caption = 'PaintBoxForm'
  ClientHeight = 645
  ClientWidth = 613
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 8
    Top = 40
    Width = 600
    Height = 600
    OnMouseMove = PaintBox1MouseMove
    OnPaint = PaintBox1Paint
  end
  object Top_Label: TLabel
    Left = 24
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
  object MainMenu1: TMainMenu
    Left = 432
    Top = 120
    object File1: TMenuItem
      Caption = 'File'
      object File2: TMenuItem
        Caption = 'Close'
        OnClick = File2Click
      end
    end
  end
end
