object StringGrid_Form: TStringGrid_Form
  Left = 0
  Top = 0
  Caption = 'Result'
  ClientHeight = 677
  ClientWidth = 872
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StringGrid1: TStringGrid
    Left = 8
    Top = 8
    Width = 856
    Height = 593
    ColCount = 6
    FixedCols = 0
    RowCount = 6
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 0
    OnKeyDown = StringGrid1KeyDown
  end
  object Close_Button: TButton
    Left = 376
    Top = 607
    Width = 129
    Height = 50
    Caption = 'Close'
    TabOrder = 1
    OnClick = Close_ButtonClick
  end
  object CopyTableHButton: TButton
    Left = 192
    Top = 607
    Width = 178
    Height = 50
    Caption = 'Copy table to clipboard (including hearders)'
    TabOrder = 2
    WordWrap = True
    OnClick = CopyTableHButtonClick
  end
  object CopyTableButton: TButton
    Left = 8
    Top = 607
    Width = 178
    Height = 50
    Caption = 'Copy table to clipboard (just values)'
    TabOrder = 3
    WordWrap = True
    OnClick = CopyTableButtonClick
  end
end
