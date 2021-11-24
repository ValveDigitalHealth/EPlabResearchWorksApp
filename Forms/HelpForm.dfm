object Help_Form: THelp_Form
  Left = 0
  Top = 0
  Caption = 'Help'
  ClientHeight = 104
  ClientWidth = 505
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 40
    Width = 111
    Height = 19
    Caption = 'For help, go to:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LinkLabel: TLabel
    Left = 157
    Top = 40
    Width = 192
    Height = 19
    Cursor = crHandPoint
    Caption = 'www.eplabworks.com/help'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnClick = LinkLabelClick
  end
  object Close_Button: TButton
    Left = 368
    Top = 32
    Width = 120
    Height = 41
    Caption = 'Close'
    TabOrder = 0
    OnClick = Close_ButtonClick
  end
end
