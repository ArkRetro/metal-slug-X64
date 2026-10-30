/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0533a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_533a2(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  *(undefined1 *)((int)unaff_A6 + 0x82) = 0;
  candidate_4256();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x533c0;
  uVar7 = 0;
  candidate_52bda();
  candidate_52b7a();
  candidate_427a();
  if ((bool)uVar7) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    *unaff_A6 = candidate_532b6;
  }
  if (*(char *)((int)unaff_A6 + 0x83) != '\0') {
    *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x92) == '\0') {
    candidate_4c24();
  }
  else {
    *(char *)((int)unaff_A6 + 0x92) = *(char *)((int)unaff_A6 + 0x92) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x5a) != '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    if (*(char *)((int)unaff_A6 + 0x83) == '\0') {
      candidate_40ed0();
      *(undefined1 *)((int)unaff_A6 + 0x83) = 0xc;
    }
    if (*(ushort *)((int)unaff_A6 + 0x4e) < *(ushort *)((int)unaff_A6 + 0x76)) {
      *(short *)((int)unaff_A6 + 0x76) =
           *(short *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x1e);
      candidate_53dcc();
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_536f4;
  }
  bVar6 = false;
  if (*(char *)((int)unaff_A6 + 0x87) == '\0') {
    candidate_19504();
    if (bVar6) {
      candidate_19dfc();
      *(undefined1 *)(unaff_A5 + 0x400b) = 0;
      *(undefined1 *)(unaff_A5 + 0x400c) = 0;
      candidate_ca2f2();
      candidate_ca2f2();
      candidate_c8ce6();
      candidate_c7b48();
      *(undefined1 *)(unaff_A5 + 0x444c) = 0;
      uVar5 = *(ushort *)(unaff_A5 + 0x700c);
      *(ushort *)(unaff_A5 + 0x700c) = uVar5 + 0x3ef;
      if (0xfc10 < uVar5) {
        uVar5 = *(ushort *)(unaff_A5 + 0x700a);
        *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
        uVar4 = uRam002fffe8;
        uVar3 = ~uVar5 & 0xfff;
        uVar5 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                         -((*(byte *)((short)(uVar5 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7)
                           ) != 0));
        uRam002fffe0 = uVar5 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
        uRam002fffe2 = 0;
        uRam002fffe4 = 0;
        uRam002fffe6 = 0;
        *(short *)(unaff_A5 + 0x700a) =
             ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
             *(short *)(unaff_A5 + 0x700a);
        if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar5 ^ uVar4) & 1) != 0) {
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
  }
  return;
}
