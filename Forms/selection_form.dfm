object Selection_List_Form: TSelection_List_Form
  Left = 536
  Top = 257
  Caption = 'Selection'
  ClientHeight = 500
  ClientWidth = 511
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 144
  TextHeight = 20
  object OK_Button: TButton
    Left = 400
    Top = 418
    Width = 103
    Height = 58
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 274
    Top = 418
    Width = 116
    Height = 58
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object Extra_Button: TButton
    Left = 8
    Top = 416
    Width = 169
    Height = 33
    Caption = '... custom...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    OnClick = Extra_ButtonClick
  end
  object Extra_Button_2: TButton
    Left = 8
    Top = 453
    Width = 169
    Height = 33
    Caption = '... custom...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Visible = False
    OnClick = Extra_Button_2Click
  end
  object ListBox: TListBox
    Left = 8
    Top = 8
    Width = 495
    Height = 402
    ItemHeight = 20
    TabOrder = 4
  end
end
