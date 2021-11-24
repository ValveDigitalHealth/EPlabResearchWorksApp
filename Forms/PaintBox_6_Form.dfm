object PaintBox_Six_Form: TPaintBox_Six_Form
  Left = 0
  Top = 0
  Align = alClient
  Caption = 'PaintBox_Six_Form'
  ClientHeight = 681
  ClientWidth = 1069
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 8
    Top = 49
    Width = 226
    Height = 208
  end
  object Label1: TLabel
    Left = 128
    Top = 8
    Width = 106
    Height = 25
    Caption = 'Top_Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 320
    Top = 26
    Width = 106
    Height = 25
    Caption = 'Top_Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 552
    Top = 8
    Width = 106
    Height = 25
    Caption = 'Top_Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 64
    Top = 280
    Width = 106
    Height = 25
    Caption = 'Top_Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 296
    Top = 271
    Width = 58
    Height = 25
    Caption = 'empty'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 560
    Top = 271
    Width = 58
    Height = 25
    Caption = 'empty'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object PaintBox2: TPaintBox
    Left = 240
    Top = 57
    Width = 226
    Height = 208
  end
  object PaintBox3: TPaintBox
    Left = 504
    Top = 57
    Width = 226
    Height = 208
  end
  object PaintBox4: TPaintBox
    Left = 8
    Top = 321
    Width = 226
    Height = 208
  end
  object PaintBox5: TPaintBox
    Left = 248
    Top = 321
    Width = 226
    Height = 208
  end
  object PaintBox6: TPaintBox
    Left = 512
    Top = 321
    Width = 226
    Height = 208
  end
  object Filename_Label: TLabel
    Left = 736
    Top = 72
    Width = 144
    Height = 25
    Caption = 'Filename_Label'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object MainMenu1: TMainMenu
    Left = 896
    Top = 336
    object File1: TMenuItem
      Caption = 'File'
      object File2: TMenuItem
        Caption = 'Close'
        OnClick = File2Click
      end
    end
  end
end
