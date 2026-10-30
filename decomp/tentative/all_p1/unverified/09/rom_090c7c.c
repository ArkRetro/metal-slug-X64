/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x090c7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_90c7c(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  byte bVar5;
  char cVar6;
  ushort uVar7;
  undefined2 unaff_D2w;
  undefined2 unaff_D3w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;

code_r0x00090c7c:
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  uVar8 = 0;
  bVar5 = *(byte *)(unaff_A6 + 0x23) & 3;
  *(byte *)(unaff_A6 + 0x23) = bVar5;
  if (bVar5 == 0) {
    candidate_91742();
    candidate_9153a();
    if ((bool)uVar8) {
      *(undefined1 *)(unaff_A6[3] + 0x61) = 1;
    }
    else {
      *(undefined1 *)(unaff_A6[3] + 0x61) = 0;
    }
  }
  bVar5 = *(byte *)(unaff_A6[3] + 0x60);
  if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
    *(byte *)(unaff_A6 + 0x18) = bVar5;
    uVar8 = 0;
    candidate_914bc();
    bVar5 = candidate_919f4();
    if ((bool)uVar8) goto LAB_00090bbe;
    uVar8 = bVar5 < *(byte *)((int)unaff_A6 + 0x92);
    if (bVar5 != *(byte *)((int)unaff_A6 + 0x92)) {
      *(byte *)((int)unaff_A6 + 0x92) = bVar5;
      *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
      bVar9 = false;
      uVar8 = candidate_919f4();
      if (bVar9) goto LAB_00090bbe;
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
      uVar8 = 0;
      candidate_4256();
    }
    candidate_427a();
    if (!(bool)uVar8) goto LAB_0009118a;
    *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
    bVar9 = false;
    uVar8 = candidate_919f4();
    if (bVar9) goto LAB_00090bbe;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
    candidate_4256();
    goto LAB_0009118a;
  }
  if (*(char *)(unaff_A6 + 0x18) == '\b') {
LAB_000911de:
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0;
    unaff_A6[0xd] = 0x66666;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff92c6;
    unaff_A6[0xc] = 0xffffbbbc;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    *unaff_A6 = 0x9120c;
    func_0x000143e4();
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_91270;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *unaff_A6 = candidate_91270;
    }
    candidate_427a();
    if (*(short *)(unaff_A6 + 0x10) < 0xc1) {
      *unaff_A6 = candidate_90986;
    }
    uVar8 = false;
    candidate_409a();
    if ((bool)uVar8) {
      *unaff_A6 = 0x9127a;
      uVar8 = 0;
    }
    candidate_91a4e();
    if ((bool)uVar8) {
      *unaff_A6 = candidate_41ac;
    }
    return;
  }
  *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
  switch(bVar5 & 0xf) {
  case 0:
LAB_00090a8e:
    *(undefined1 *)((int)unaff_A6 + 0x91) = 8;
    *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
    bVar9 = false;
    uVar8 = candidate_919f4();
    if (bVar9) goto LAB_00090bbe;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
    candidate_4256();
    *unaff_A6 = 0x90ade;
    bVar5 = *(byte *)(unaff_A6[3] + 0x60);
    if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
      *(byte *)(unaff_A6 + 0x18) = bVar5;
      uVar8 = 0;
      candidate_914bc();
      bVar5 = candidate_919f4();
      if (!(bool)uVar8) {
        uVar8 = bVar5 < *(byte *)((int)unaff_A6 + 0x92);
        if (bVar5 != *(byte *)((int)unaff_A6 + 0x92)) {
          *(byte *)((int)unaff_A6 + 0x92) = bVar5;
          *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
          bVar9 = false;
          uVar8 = candidate_919f4();
          if (bVar9) goto LAB_00090bbe;
          *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
          uVar8 = 0;
          candidate_4256();
        }
        candidate_427a();
        if (!(bool)uVar8) goto LAB_0009118a;
        *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
        bVar9 = false;
        uVar8 = candidate_919f4();
        if (!bVar9) {
          *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
          candidate_4256();
          goto LAB_0009118a;
        }
      }
      goto LAB_00090bbe;
    }
    if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
    *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
    switch(bVar5 & 0xf) {
    case 0:
      goto LAB_00090a8e;
    case 1:
LAB_00090bbe:
      *(undefined1 *)((int)unaff_A6 + 0x91) = 8;
      candidate_4256();
      *unaff_A6 = 0x90bda;
      bVar5 = *(byte *)(unaff_A6[3] + 0x60);
      if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
        *(byte *)(unaff_A6 + 0x18) = bVar5;
        uVar8 = 0;
        candidate_914bc();
        candidate_919f4();
        if ((bool)uVar8) {
          candidate_427a();
          return;
        }
        goto LAB_00090a8e;
      }
      if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
      *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
      switch(bVar5 & 0xf) {
      case 0:
        goto LAB_00090a8e;
      case 1:
        goto LAB_00090bbe;
      case 2:
code_r0x00090c20:
        *(undefined1 *)(unaff_A6 + 0x23) = 0;
        *(undefined4 *)((int)unaff_A6 + 0x76) = *(undefined4 *)(unaff_A6[3] + 0x76);
        func_0x0009174e();
        *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
        bVar9 = false;
        uVar8 = candidate_919f4();
        if (!bVar9) {
          *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
          candidate_4256();
          *unaff_A6 = candidate_90c7c;
          goto code_r0x00090c7c;
        }
        goto LAB_00090bbe;
      default:
code_r0x00090d8c:
        *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
        bVar9 = false;
        uVar8 = candidate_919f4();
        if (bVar9) goto LAB_00090bbe;
        *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
        candidate_4256();
        func_0x00091b02();
        *unaff_A6 = candidate_90dc4;
        bVar5 = *(byte *)(unaff_A6[3] + 0x60);
        if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
          *(byte *)(unaff_A6 + 0x18) = bVar5;
          uVar8 = 0;
          candidate_914bc();
          candidate_427a();
          if ((bool)uVar8) {
            *(undefined1 *)(unaff_A6[3] + 0x61) = 1;
          }
          return;
        }
        if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
        *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
        switch(bVar5 & 0xf) {
        case 0:
          goto LAB_00090a8e;
        case 1:
          goto LAB_00090bbe;
        case 2:
          goto code_r0x00090c20;
        default:
          goto code_r0x00090d8c;
        case 5:
          break;
        case 6:
          goto code_r0x00090f78;
        case 7:
          goto code_r0x000910a6;
        case 8:
          goto LAB_000911de;
        case 9:
          goto code_r0x00090986;
        }
      case 5:
        break;
      case 6:
        goto code_r0x00090f78;
      case 7:
        goto code_r0x000910a6;
      case 8:
        goto LAB_000911de;
      case 9:
        goto code_r0x00090986;
      }
    case 2:
      goto code_r0x00090c20;
    default:
      goto code_r0x00090d8c;
    case 5:
      break;
    case 6:
      goto code_r0x00090f78;
    case 7:
      goto code_r0x000910a6;
    case 8:
      goto LAB_000911de;
    case 9:
      goto code_r0x00090986;
    }
  case 1:
    goto LAB_00090bbe;
  case 2:
    goto code_r0x00090c20;
  default:
    goto code_r0x00090d8c;
  case 5:
    break;
  case 6:
    goto code_r0x00090f78;
  case 7:
    goto code_r0x000910a6;
  case 8:
    goto LAB_000911de;
  case 9:
code_r0x00090986:
    uVar7 = *(ushort *)(unaff_A5 + 0x700c);
    *(ushort *)(unaff_A5 + 0x700c) = uVar7 + 0x3ef;
    if (0xfc10 < uVar7) {
      uVar7 = *(ushort *)(unaff_A5 + 0x700a);
      *(short *)(unaff_A5 + 0x700a) = *(short *)(unaff_A5 + 0x700a) + 1;
      uVar4 = uRam002fffe8;
      uVar3 = ~uVar7 & 0xfff;
      uVar7 = CONCAT11((char)((ushort)unaff_D2w >> 8),
                       -((*(byte *)((short)(uVar7 >> 3 & 0xfff) + 0xdedd2) & '\x01' << (uVar3 & 7))
                        != 0));
      uRam002fffe0 = uVar7 & CONCAT11((char)((ushort)unaff_D3w >> 8),-(0x7ff < uVar3));
      uRam002fffe2 = 0;
      uRam002fffe4 = 0;
      uRam002fffe6 = 0;
      *(short *)(unaff_A5 + 0x700a) =
           ((short)-(uVar3 == 0x400) & _DAT_003c0006 + *(short *)(unaff_A5 + -0x7ff2) & 0xf3ffU) +
           *(short *)(unaff_A5 + 0x700a);
      if (((ushort)(byte)-(0xfbff < (ushort)(uVar3 - 0x800)) & (uVar7 ^ uVar4) & 1) != 0) {
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
code_r0x00090e10:
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
  bVar9 = false;
  uVar8 = candidate_919f4();
  if (bVar9) goto LAB_00090bbe;
  *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
  candidate_4256();
  *unaff_A6 = candidate_90e5e;
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  bVar5 = *(byte *)(unaff_A6 + 0x23) & 3;
  *(byte *)(unaff_A6 + 0x23) = bVar5;
  if (bVar5 == 0) {
    if (*(char *)((int)unaff_A6 + 0x91) == '\b') {
      *(undefined1 *)(unaff_A6[3] + 0x61) = 1;
    }
    else {
      if (*(char *)((int)unaff_A6 + 0x91) < '\b') {
        cVar6 = '\x01';
      }
      else {
        cVar6 = -1;
      }
      *(char *)((int)unaff_A6 + 0x91) = cVar6 + *(char *)((int)unaff_A6 + 0x91);
    }
  }
  bVar5 = *(byte *)(unaff_A6[3] + 0x60);
  if (bVar5 != *(byte *)(unaff_A6 + 0x18)) {
    if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
    *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
    switch(bVar5 & 0xf) {
    case 0:
      goto LAB_00090a8e;
    case 1:
      goto LAB_00090bbe;
    case 2:
      goto code_r0x00090c20;
    default:
      goto code_r0x00090d8c;
    case 5:
      goto code_r0x00090e10;
    case 6:
code_r0x00090f78:
      *(undefined1 *)(unaff_A6 + 0x23) = 0;
      *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
      bVar9 = false;
      uVar8 = candidate_919f4();
      if (bVar9) goto LAB_00090bbe;
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
      candidate_4256();
      *unaff_A6 = candidate_90fc6;
      bVar5 = *(byte *)(unaff_A6[3] + 0x60);
      if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
        *(byte *)(unaff_A6 + 0x18) = bVar5;
        uVar8 = 0;
        candidate_914bc();
        bVar5 = candidate_919f4();
        if ((bool)uVar8) goto LAB_00090bbe;
        uVar8 = bVar5 < *(byte *)((int)unaff_A6 + 0x92);
        if (bVar5 != *(byte *)((int)unaff_A6 + 0x92)) {
          *(byte *)((int)unaff_A6 + 0x92) = bVar5;
          *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
          bVar9 = false;
          uVar8 = candidate_919f4();
          if (bVar9) goto LAB_00090bbe;
          *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
          uVar8 = 0;
          candidate_4256();
        }
        candidate_427a();
        if (!(bool)uVar8) goto LAB_0009118a;
        *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
        bVar9 = false;
        uVar8 = candidate_919f4();
        if (bVar9) goto LAB_00090bbe;
        *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
        candidate_4256();
        goto LAB_0009118a;
      }
      if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
      *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
      switch(bVar5 & 0xf) {
      case 0:
        goto LAB_00090a8e;
      case 1:
        goto LAB_00090bbe;
      case 2:
        goto code_r0x00090c20;
      default:
        goto code_r0x00090d8c;
      case 5:
        goto code_r0x00090e10;
      case 6:
        goto code_r0x00090f78;
      case 7:
code_r0x000910a6:
        *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
        bVar9 = false;
        uVar8 = candidate_919f4();
        if (bVar9) goto LAB_00090bbe;
        *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
        candidate_4256();
        *unaff_A6 = candidate_910f0;
        bVar5 = *(byte *)(unaff_A6[3] + 0x60);
        if (bVar5 == *(byte *)(unaff_A6 + 0x18)) {
          *(byte *)(unaff_A6 + 0x18) = bVar5;
          uVar8 = 0;
          candidate_914bc();
          cVar6 = candidate_919f4();
          if ((bool)uVar8) goto LAB_00090bbe;
          if (cVar6 != *(char *)((int)unaff_A6 + 0x92)) {
            *(char *)((int)unaff_A6 + 0x92) = cVar6;
            *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
            bVar9 = false;
            uVar8 = candidate_919f4();
            if (bVar9) goto LAB_00090bbe;
            *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
            candidate_4256();
          }
          *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
          candidate_427a();
          return;
        }
        if (*(char *)(unaff_A6 + 0x18) == '\b') goto LAB_000911de;
        *(byte *)(unaff_A6 + 0x18) = bVar5;
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
        switch(bVar5 & 0xf) {
        case 0:
          goto LAB_00090a8e;
        case 1:
          goto LAB_00090bbe;
        case 2:
          goto code_r0x00090c20;
        default:
          goto code_r0x00090d8c;
        case 5:
          goto code_r0x00090e10;
        case 6:
          goto code_r0x00090f78;
        case 7:
          goto code_r0x000910a6;
        case 8:
          goto LAB_000911de;
        case 9:
          goto code_r0x00090986;
        }
      case 8:
        goto LAB_000911de;
      case 9:
        goto code_r0x00090986;
      }
    case 7:
      goto code_r0x000910a6;
    case 8:
      goto LAB_000911de;
    case 9:
      goto code_r0x00090986;
    }
  }
  *(byte *)(unaff_A6 + 0x18) = bVar5;
  uVar8 = 0;
  candidate_914bc();
  bVar5 = candidate_919f4();
  if ((bool)uVar8) goto LAB_00090bbe;
  uVar8 = bVar5 < *(byte *)((int)unaff_A6 + 0x92);
  if (bVar5 != *(byte *)((int)unaff_A6 + 0x92)) {
    *(byte *)((int)unaff_A6 + 0x92) = bVar5;
    *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
    bVar9 = false;
    uVar8 = candidate_919f4();
    if (bVar9) goto LAB_00090bbe;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
    uVar8 = 0;
    candidate_4256();
  }
  candidate_427a();
  if (!(bool)uVar8) goto LAB_0009118a;
  *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
  bVar9 = false;
  uVar8 = candidate_919f4();
  if (bVar9) goto LAB_00090bbe;
  *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
  candidate_4256();
LAB_0009118a:
  if ((*(byte *)(unaff_A6[3] + 0x96) & 0x40) != 0) {
    *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(unaff_A6[3] + 0x90);
    bVar9 = false;
    uVar8 = candidate_919f4();
    if (bVar9) goto LAB_00090bbe;
    *(undefined1 *)((int)unaff_A6 + 0x92) = uVar8;
    candidate_4256();
  }
  return;
}
