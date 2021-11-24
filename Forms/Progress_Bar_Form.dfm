object Progress_Form: TProgress_Form
  Left = 776
  Top = 384
  Caption = 'Please wait...'
  ClientHeight = 465
  ClientWidth = 569
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object RichEditMain: TRichEdit
    Left = 8
    Top = 8
    Width = 553
    Height = 401
    Color = clWhite
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Consolas'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    Zoom = 100
  end
  object Cancel_Button: TButton
    Left = 8
    Top = 415
    Width = 553
    Height = 42
    Caption = 'CLOSE'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
end
