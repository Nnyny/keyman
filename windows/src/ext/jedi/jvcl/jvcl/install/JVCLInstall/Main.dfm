object FormMain: TFormMain
  Left = 214
  Top = 150
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'JVCL 3  Installation'
  ClientHeight = 493
  ClientWidth = 683
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  DesignSize = (
    683
    493)
  PixelsPerInch = 96
  TextHeight = 13
  object LblHomepage: TLabel
    Left = 8
    Top = 473
    Width = 201
    Height = 14
    Cursor = crHandPoint
    Hint = '<c:blue>https://github.com/project-jedi/jvcl'
    Anchors = [akLeft, akBottom]
    Caption = 'JEDI Visual Component Library Homepage'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    OnClick = LblHomepageClick
  end
  object PanelLogo: TPanel
    Left = 23
    Top = 8
    Width = 116
    Height = 31
    AutoSize = True
    BevelOuter = bvNone
    Color = clWindow
    TabOrder = 0
    object ImageLogo: TImage
      Left = 0
      Top = 0
      Width = 116
      Height = 31
      Cursor = crHandPoint
      AutoSize = True
      Picture.Data = {
        0A544A504547496D616765650B0000FFD8FFE000104A46494600010100000100
        010000FFDB0043000302020302020303030304030304050805050404050A0707
        06080C0A0C0C0B0A0B0B0D0E12100D0E110E0B0B1016101113141515150C0F17
        1816141812141514FFDB00430103040405040509050509140D0B0D1414141414
        1414141414141414141414141414141414141414141414141414141414141414
        14141414141414141414141414FFC0001108001F007403012200021101031101
        FFC4001F0000010501010101010100000000000000000102030405060708090A
        0BFFC400B5100002010303020403050504040000017D01020300041105122131
        410613516107227114328191A1082342B1C11552D1F02433627282090A161718
        191A25262728292A3435363738393A434445464748494A535455565758595A63
        6465666768696A737475767778797A838485868788898A92939495969798999A
        A2A3A4A5A6A7A8A9AAB2B3B4B5B6B7B8B9BAC2C3C4C5C6C7C8C9CAD2D3D4D5D6
        D7D8D9DAE1E2E3E4E5E6E7E8E9EAF1F2F3F4F5F6F7F8F9FAFFC4001F01000301
        01010101010101010000000000000102030405060708090A0BFFC400B5110002
        0102040403040705040400010277000102031104052131061241510761711322
        328108144291A1B1C109233352F0156272D10A162434E125F11718191A262728
        292A35363738393A434445464748494A535455565758595A636465666768696A
        737475767778797A82838485868788898A92939495969798999AA2A3A4A5A6A7
        A8A9AAB2B3B4B5B6B7B8B9BAC2C3C4C5C6C7C8C9CAD2D3D4D5D6D7D8D9DAE2E3
        E4E5E6E7E8E9EAF2F3F4F5F6F7F8F9FAFFDA000C03010002110311003F00FA77
        E3CFC64D2FE0B6B6F73E2DF1CEA9E1FB3D4AFAE16C9524BB914846C9555895B6
        80197AE073C571BE03FDA7FC1DF12B5FB6D0FC3DF156FAF356B96D90DACD7377
        6ED2B609DABE6AAEE6C06E073C575DFB757ECF5A17C6C1E10B5D5757D43487B7
        7BD9E096C228A4DD2308461D5D97231FDD39CD78D7887F613F07697E3AF859AC
        F877C449A05DE8DA56912DC5BC36689F6FBB864DED7121F306C964C2E40C9C80
        4649A4F1118BB12A8392B9F491D3BC5249FF008A875903D46A12FF00F15EF48F
        63E274049F11EB0A39EBA84A07FE85F5AF83AE3F601F885A9687F11358835D3A
        D6B568D6B79E1DB5D17535297A25B9613A4E24DAD1491C65580CED24E159F15B
        3F1C3E1B7C69F887F0FBF67ED03C49A7F8AAF6E624BA83C457362925E1808BE3
        0473DCBC45D0C82DD01F31C93CC849CB355AAD16AE4FB168FB6058789DD72BE2
        3D61811C11A84BCFFE3D4A74EF1460E3C43ACFB7FA7CBEFF00ED7D2BE69FD863
        E1678DFE127ED61F11BC17A85AF8A53E1F476D7D169F7DACD94B1DADEBC1771A
        5BCA923208D9CC4D29FDDE0302C70428C72BE277BEB5FF0082927C45B1FB55C2
        D926877EF1C5E6B08F8D177021738FBC3B77155ED09F647D81FD9DE28CFF00C8
        C5ACE39FF97F97DFFDAA69B2F1316DBFF0926AFBCF45FED097D3FDEFAFE55F9E
        BFB0A687E15D6F59FF00849757F893A969DF10F489EEA6D17C2FE6174D4923B2
        79096C83E920C6402171C93CF1FF0003FF00678F1DFED25F0F7C63E2FF000C6A
        DACEAFE3BD2B57B44480EA2B11992612BCD33CD2306F3032A904367E63D7390F
        9FC83D9799FA7874EF1413FF002316B207B5FCBEFF00ED5034EF147FD0C5ACFF
        00E07CBEBFEF7D2BF3AFF69AFD9F359F85FF00117E175BEB5E27D67FE123F886
        63BEF10C2D3205B1BE9E68FED29088FE5DAB24B263A8E00C91D7F563E10FC11B
        2F849F0E345F0945AB5E6B71E991BC6B7F7D8F3A50D233FCD8E38DD8FA014738
        9D3F33E76F07FC74D1FC7BE3DD53C17A0FC43D5EFF00C4DA599C5DD8092F13CA
        F264114BF3BA843B5C81C31CF51919AF45FECEF14E3FE462D6738E9F6F97FF00
        8AAFC9CF194D3587C54FDA267B69E48278AE750C340C430FF89F5A83C8E79C9E
        FEB5ED5FB2EF84BC11A87C0CF885E293F132FAE3E24C7E09F11F9FE10926CA24
        2B0CAAB3292324EDF2DF827AE7A03839C7ECD773EF93A778A3903C43ACFB6750
        97FF008AA0E9FE27DDFF002316B3F4FB7CBFFC557E393C3AF782FE11F85BE276
        93E2DD6EC75E9FC4B7FA5AAC576C1625B7B7B49124420E771FB4BAB0390401C7
        273F465D7C62D4FC29FF000503BEF181D3B54D611F47B7BC4D0F4E0F2896E2E3
        4389A38C460F4F32404F700120122AA327276483D9F99C37FC14E7C55E2DF0AF
        C7CD0AD62F14EB91093C356F31035198649BABA5CFDEFF0066BE44FF0085A1E3
        2FFA1B75DFFC194DFF00C557ACFEDB4DE37BCF8C169AAF8EF51B7D5F50D5B47B
        7D42CAFAC67596D25B47793CBF20AF011583AE0018657CE4E49F00A8A91E5934
        6915647F447FB4E5D5B5E5DF87B7C8635B6FB52B30708324C3DC823F84FF0091
        527C3DF853A5598D2759D4358D42E1B6C77634D4C431AB9018090E37B6D3C632
        A08C8652381F3E78FF00FE0A0BF0634FF186B9A75EF8A959ED6FA68CA49A4DDB
        804391C7EE48E95909FF000521F8311A2AAF8C595546028D26F4003D3FD4D64A
        8C79F9DB13A92B5923DBB45B4BB9FC3DA85F36B1716CF6290A0B79465652ED8C
        13B32A071B76F3C9CE7E523A0FEDB92E7C3DE1E8634F2D8DBCC922D933E1B13B
        0672173BCB64162D9DC5989C939AF9987FC1427E04AA5CA0F11C012E7FD7A8D1
        2EC097FDEFDC7CDD4F5F5351DDFF00C1403E025FE9CB6173AFDADC582FDDB597
        42BA688739E14C18EB59AC3D9594BFAB95ED9F667D5575F115B4EF125E5CC16A
        2D5D659678DA689C3B42CFB77608CFDE9173C606473C9AB6BAD45AE7892DAE2E
        A354BB95660D14B9567536B2A950A7EF72C338E99FAD7C91A6FEDFBF00B45B69
        2DB4FD7AD2CADE4003C36DA15D468E06700810007A9FCE8D2BF6FEF805A0A4AB
        A66BB69A7ACB9F305A68375107CF5CED8067F1AA5476BC85ED5F667D25E15F84
        1F092D3C5E9E256F04E8B67ABDB8F26D6EF4DB116843CA258E4DDE56D5766590
        AE58124679E38E83E10781FC05F066DB5983C0DA52786ACEF648A5BB28CCEA4A
        07196DECDB40CE327D3BD7C9B6FF00B7F7C02B39925835DB4865439478F42BA5
        653EA08838A86EFF006EFF00D9E6FEF8DEDD6ABA75CDE16DC6E26F0F5CBC99F5
        DC6DF39F7AA54AC97BC1ED5DF667D15F1FFE06784FE3F4FE0FF1EEAFA96AFA76
        B9E18B35BFB3B785A382390A959F6CD1C913383B900C06520161D45753F1DFC3
        1E2EF1CF88FC337BE0FF0089FA8F8185938F3F4DB5B5F3E2D40798A4EFC10C30
        3E5E011F373E87E5BBBFF82887C0DBF78DEEBC4F15CBC7F71A6D16EDCAF7E098
        78E40FCA9F7BFF000516F823A8DBBC177E2B5BA81F868A6D1EF1D5BEA0C38AB7
        0DF517B47A68707E27FD827C6BAC7C41F8A93D978AFC1D2378C16F2F2CA037B7
        41A08DF54B6BA067616C563F917681924B1E32158AFA17C2DFD996DBE0E7ECD3
        E31B0D4FE1F5CDC7C58D47C31E21D362D774A9C5E4575E6C13982DD16394B33B
        26C555F28312B8EB8CD2B3FF0082847C09D3ED24B3B5F11C16B692643C30E897
        691BE460E54418391ED589E25FF8289FC1AF0F6877577E1CB9B7D57570A56DED
        63D267B70598632CED10017079C724700734D41DD6A839EFF64F27F15683F0EB
        C43F08F4DF83363E00F883E1EF1DDB5D477DA645ADDA182F2F355B958E394B5B
        BBED10491DBC60107F779042B6097FA3FE03FC233A1F8DA693C5C9617DF13B55
        B546D38BDC24490A5B0090B693791B32C81115629410644280B2C885B7FCF5F0
        93F680F807A825C78BFC73E3DBBD37C7FABCF27DAFCBD12E146906370D6B71A7
        4D046C6274DABF7B707C147500EFA93E26FEDA3F0E3C7F6EBE0C6D6EDECA0FED
        58F51BEF1C69F67791C7249167FD2AC2D3C9125A5C4E180987DCCAB119DC49F4
        23C91F7535E6FF00AFE9892B1F3E7FC1426FBC1BA8FC7B8EE3C186E6181F4B8C
        EA9A7CF1794B63A97DA27FB4C31A0255577FCE4464A6F91F69208AF99ABDE7F6
        C6F1C7803C6FE3DF0C9F86D324DE1CD2BC3D0E9DB96DA5849996E2E2462DE62A
        B33112AB16C724F5AF06AE0A8D39368B5AA3FFD9}
      OnClick = ImageLogoClick
    end
  end
  object ImageList: TImageList
    Left = 624
    Top = 8
    Bitmap = {
      494C010101000500040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF000000000000FEFF000000000000FC7F000000000000
      F83F000000000000F01F000000000000F83F000000000000FC7F000000000000
      FEFF000000000000FFFF000000000000FFFF000000000000FFFF000000000000
      FFFF000000000000FFFF00000000000000000000000000000000000000000000
      000000000000}
  end
  object ImageListCheckMark: TImageList
    Left = 592
    Top = 8
    Bitmap = {
      494C010101000500040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000800000008000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080000000A0000000A000000080000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000080000000A0000000A000000080000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000080000000A0000000A0000000A0000000A0000000800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000080
      000000A0000000A000000080000000A0000000A0000000800000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000080000000A0
      00000080000000800000000000000080000000A0000000A00000008000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000800000008000000080
      0000000000000000000000000000000000000080000000A00000008000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000080000000A0000000A000000080
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000080000000A000000080
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000080000000A0
      0000008000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000080
      000000A000000080000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008000000080000000800000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000FCFF000000000000
      F87F000000000000F87F000000000000F03F000000000000E03F000000000000
      C21F0000000000008F1F000000000000FF0F000000000000FF8F000000000000
      FFC7000000000000FFE3000000000000FFF1000000000000FFFF000000000000
      FFFF000000000000FFFF00000000000000000000000000000000000000000000
      000000000000}
  end
end