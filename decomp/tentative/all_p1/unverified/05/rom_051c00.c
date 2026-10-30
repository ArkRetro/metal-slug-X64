/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x051c00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_51c00(void)

{
  code *pcVar1;
  int iVar2;
  char *pcVar3;
  char cVar4;
  char cVar5;
  ushort uVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  ushort uVar8;
  undefined2 unaff_D3w;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined2 in_stack_fffffffa;
  undefined2 uVar9;

  *(undefined4 *)((int)unaff_A6 + 0x76) = 0x50f50;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0xff;
  candidate_1ad6e();
  candidate_50fb4();
  *unaff_A6 = 0x51c2a;
  uVar9 = 0x1c30;
  candidate_1abc8();
  *(char *)((int)unaff_A6 + 0x9a) = *(char *)((int)unaff_A6 + 0x9a) + '\x01';
  if (*(char *)((int)unaff_A6 + 0x9a) == '\0') {
    *(undefined1 *)(unaff_A6[3] + 0xa1) = *(undefined1 *)(unaff_A6[3] + 0x1a);
  }
  if (*(char *)((int)unaff_A6 + 0x9a) == '\x01') {
    *(undefined1 *)(unaff_A6[3] + 0xa1) = *(undefined1 *)(unaff_A6[3] + 0x19);
  }
  if (*(char *)((int)unaff_A6 + 0x9a) == '\b') {
    *(undefined1 *)(unaff_A6[3] + 0xa1) = *(undefined1 *)(unaff_A6[3] + 0x1a);
  }
  if (*(char *)((int)unaff_A6 + 0x9a) == '\t') {
    *(undefined1 *)(unaff_A6[3] + 0xa1) = *(undefined1 *)(unaff_A6[3] + 0x19);
  }
  if ((*(byte *)((int)unaff_A6 + 0x9a) & 1) != 0) {
    pcVar3 = *(char **)((int)unaff_A6 + 0x76);
    cVar4 = *pcVar3;
    cVar5 = pcVar3[1];
    if (cVar5 < '\0') {
      *(undefined1 *)(unaff_A6[3] + 0x99) = 0xff;
      uVar8 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar8 + 0x3ef;
      if (0xfc10 < uVar8) {
        uVar8 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar7 = uRam002fffe8;
        uVar6 = ~uVar8 & 0xfff;
        uVar8 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar8 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar6 & 7)
                           ) != 0));
        uRam002fffe0 = uVar8 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar6));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar6 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar6 - 0x800)) & (uVar8 ^ uVar7) & 1) != 0) {
          *(undefined2 *)(unaff_A5 + -0x7fec) = 1;
          _DAT_002ffff0 = 1;
          do {
          } while (DAT_002fffff != '\x01');
          uRam00400000 = 0;
          uRam00400002 = *(undefined2 *)((short)(sRam002112e2 * 2) + 0x224000);
          uRam00400004 = *(undefined2 *)((short)(sRam002112e4 * 2) + 0x224000);
          uRam00400006 = *(undefined2 *)((short)(sRam002112e6 * 2) + 0x224000);
          uRam00400008 = *(undefined2 *)((short)(sRam002112e8 * 2) + 0x224000);
          uRam0040000a = *(undefined2 *)((short)(sRam002112ea * 2) + 0x224000);
          uRam0040000c = *(undefined2 *)((short)(sRam002112ec * 2) + 0x224000);
          uRam0040000e = *(undefined2 *)((short)(sRam002112ee * 2) + 0x224000);
          uRam00400010 = *(undefined2 *)((short)(sRam002112f0 * 2) + 0x224000);
          uRam00400012 = *(undefined2 *)((short)(sRam002112f2 * 2) + 0x224000);
          uRam00400014 = *(undefined2 *)((short)(sRam002112f4 * 2) + 0x224000);
          uRam00400016 = *(undefined2 *)((short)(sRam002112f6 * 2) + 0x224000);
          uRam00400018 = *(undefined2 *)((short)(sRam002112f8 * 2) + 0x224000);
          uRam0040001a = *(undefined2 *)((short)(sRam002112fa * 2) + 0x224000);
          uRam0040001c = *(undefined2 *)((short)(sRam002112fc * 2) + 0x224000);
          uRam0040001e = *(undefined2 *)((short)(sRam002112fe * 2) + 0x224000);
          _DAT_00401ffe = 0;
          func_0x0003cf12();
          candidate_3e2a8();
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
      }
      candidate_4126();
      iVar2 = *(int *)(unaff_A5 + -0x1c24);
      do {
        unaff_A6 = (undefined4 *)unaff_A6[2];
        pcVar1 = (code *)*unaff_A6;
        *(undefined4 *)(iVar2 + -4) = 0x424a;
        (*pcVar1)();
      } while( true );
    }
    *(char **)((int)unaff_A6 + 0x76) = pcVar3 + 2;
    candidate_40bc(CONCAT22((short)cVar4,in_stack_fffffffa),CONCAT22((short)cVar5,uVar9));
    candidate_1acf2();
    *(undefined2 *)(extraout_A0 + 0x1e) = *(undefined2 *)((int)unaff_A6 + 0x1e);
    *(undefined2 *)(extraout_A0 + 0x66) = *(undefined2 *)((int)unaff_A6 + 0x66);
    candidate_120f8();
  }
  return;
}
