object Ask_For_Single_Item_Form1: TAsk_For_Single_Item_Form1
  Left = 0
  Top = 0
  Caption = 'Input'
  ClientHeight = 70
  ClientWidth = 546
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 144
  TextHeight = 19
  object Edit_Box: TEdit
    Left = 8
    Top = 15
    Width = 265
    Height = 27
    TabOrder = 0
    Text = 'Edit_Box'
  end
  object OK_Button: TButton
    Left = 420
    Top = 8
    Width = 110
    Height = 41
    Caption = 'OK'
    TabOrder = 1
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 292
    Top = 8
    Width = 110
    Height = 41
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Cancel_ButtonClick
  end
end
