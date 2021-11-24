object Gemetric_Transformation_Form: TGemetric_Transformation_Form
  Left = 336
  Top = 307
  Caption = 'Geometrical transformation'
  ClientHeight = 209
  ClientWidth = 684
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 144
  TextHeight = 20
  object Label1: TLabel
    Left = 82
    Top = 41
    Width = 122
    Height = 20
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Translation X Y Z'
  end
  object Label2: TLabel
    Left = 86
    Top = 90
    Width = 118
    Height = 20
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Rotation  Teta Fi'
  end
  object Label3: TLabel
    Left = 86
    Top = 139
    Width = 119
    Height = 20
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Scaling for X Y Z'
  end
  object Label4: TLabel
    Left = 480
    Top = 49
    Width = 110
    Height = 20
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'For segment id:'
  end
  object Label5: TLabel
    Left = 480
    Top = 73
    Width = 143
    Height = 20
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = '(0 - whole geometry)'
  end
  object X_Edit: TEdit
    Left = 222
    Top = 37
    Width = 50
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 0
    Text = '0'
  end
  object Y_Edit: TEdit
    Left = 283
    Top = 37
    Width = 51
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 1
    Text = '0'
  end
  object Z_Edit: TEdit
    Left = 342
    Top = 38
    Width = 50
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 2
    Text = '0'
  end
  object Teta_Edit: TEdit
    Left = 222
    Top = 86
    Width = 50
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 3
    Text = '0'
  end
  object Fi_Edit: TEdit
    Left = 283
    Top = 86
    Width = 51
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 4
    Text = '0'
  end
  object aX_Edit: TEdit
    Left = 222
    Top = 135
    Width = 50
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 5
    Text = '1'
    OnChange = aX_EditChange
  end
  object aY_Edit: TEdit
    Left = 283
    Top = 135
    Width = 51
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 6
    Text = '1'
  end
  object OK_Button: TButton
    Left = 553
    Top = 140
    Width = 115
    Height = 39
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'OK'
    TabOrder = 7
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 428
    Top = 140
    Width = 115
    Height = 39
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    Caption = 'Cancel'
    TabOrder = 8
    OnClick = Cancel_ButtonClick
  end
  object aZ_Edit: TEdit
    Left = 342
    Top = 136
    Width = 50
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 9
    Text = '1'
  end
  object Seg_No_Edit: TEdit
    Left = 593
    Top = 46
    Width = 51
    Height = 28
    Margins.Left = 5
    Margins.Top = 5
    Margins.Right = 5
    Margins.Bottom = 5
    TabOrder = 10
    Text = '0'
  end
end
