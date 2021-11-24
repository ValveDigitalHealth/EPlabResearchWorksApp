object About_Window_Form1: TAbout_Window_Form1
  Left = 0
  Top = 0
  Caption = 'Welcome'
  ClientHeight = 661
  ClientWidth = 827
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 144
  TextHeight = 19
  object Info_RichEdit: TRichEdit
    Left = 8
    Top = 8
    Width = 811
    Height = 585
    BevelKind = bkTile
    BevelWidth = 3
    Color = clInactiveBorder
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'EPLab Research Works'
      ''
      'This product is licensed to:'
      ''
      '[Licensee info]'
      ''
      'For research purposes only.'
      ''
      'Copyright (c) 2019 Valve Digital  Health Software '
      'House, All rights reserved')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 1
    Zoom = 100
  end
  object Close_Button: TButton
    Left = 8
    Top = 599
    Width = 808
    Height = 54
    Caption = 'Continue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = Close_ButtonClick
  end
end
