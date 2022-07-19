object Directory_Selection_Form: TDirectory_Selection_Form
  Left = 0
  Top = 0
  Caption = 'Directory_Selection_Form'
  ClientHeight = 463
  ClientWidth = 645
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object DirectoryListBox: TDirectoryListBox
    Left = 24
    Top = 16
    Width = 441
    Height = 425
    TabOrder = 0
  end
  object Close_Button: TButton
    Left = 480
    Top = 392
    Width = 137
    Height = 49
    Caption = 'Select'
    TabOrder = 1
    OnClick = Close_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 542
    Top = 361
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Cancel_ButtonClick
  end
end
