object AskFor_StartStopStep_Form: TAskFor_StartStopStep_Form
  Left = 0
  Top = 0
  Caption = 'Query'
  ClientHeight = 184
  ClientWidth = 464
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
    Left = 40
    Top = 40
    Width = 97
    Height = 19
    Caption = 'Starting value'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 68
    Top = 80
    Width = 69
    Height = 19
    Caption = 'End value'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 106
    Top = 120
    Width = 31
    Height = 19
    Caption = 'Step'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Start_Value_Edit: TEdit
    Left = 160
    Top = 37
    Width = 89
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
  end
  object Stop_Value_Edit: TEdit
    Left = 160
    Top = 77
    Width = 89
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '1'
  end
  object Step_Value_Edit: TEdit
    Left = 160
    Top = 117
    Width = 89
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '0.1'
  end
  object Close_Button: TButton
    Left = 296
    Top = 38
    Width = 129
    Height = 109
    Caption = 'CLOSE'
    TabOrder = 3
    OnClick = Close_ButtonClick
  end
end
