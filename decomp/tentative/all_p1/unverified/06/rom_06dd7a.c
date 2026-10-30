/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06dd7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6dd7a(void)

{
  byte bVar1;
  undefined2 uVar4;
  undefined4 uVar2;
  short sVar5;
  uint uVar3;
  int iVar6;
  int iVar7;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;

  if (*(short *)(unaff_A6 + 0xc) == 0) {
    candidate_6e2b6();
  }
  else {
    candidate_6e2b6();
  }
  candidate_1b1dc();
  *unaff_A6 = 0x6dda2;
  uVar8 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_0006dcd6:
    sVar5 = *(short *)(unaff_A6 + 0xc);
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      sVar5 = -sVar5;
    }
    if (sVar5 < 1) {
      candidate_6e2b6();
    }
    else {
      candidate_6e2b6();
    }
    *unaff_A6 = 0x6dd06;
    unaff_A6[0xd] = (-0x160000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
    uVar3 = candidate_6e230();
    iVar7 = 0;
    iVar6 = 5;
    if ((uVar3 & 4) != 0) {
      iVar7 = -0x40000;
      iVar6 = 2;
    }
    if ((uVar3 & 8) != 0) {
      iVar7 = 0x40000;
      iVar6 = 2;
    }
    unaff_A6[0xc] = (iVar7 - unaff_A6[0xc] >> iVar6) + unaff_A6[0xc];
    candidate_143ee();
    candidate_6e27a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_6dd7a;
    }
    candidate_6e390();
    if (*(short *)(unaff_A6 + 0xd) < 0) {
      *(undefined4 *)((int)unaff_A6 + 0x6e) = 0;
    }
  }
  else {
    uVar3 = candidate_6e230();
    if ((uVar3 & 0x80) != 0) {
LAB_0006ddf2:
      candidate_1b1dc();
      candidate_aa7fc();
      candidate_6e2b6();
      uVar2 = 0x60000;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        uVar2 = 0xfffa0000;
      }
      *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
      *unaff_A6 = 0x6de2e;
      uVar8 = 0;
      candidate_6e258();
      candidate_1945c();
      if ((bool)uVar8) {
        *unaff_A6 = candidate_6df7e;
      }
      candidate_6e27a();
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
        *unaff_A6 = candidate_6de6c;
      }
      candidate_49e8();
      goto LAB_0006df76;
    }
    if ((uVar3 & 2) == 0) {
      if ((uVar3 & 4) != 0) goto LAB_0006dbf2;
      if ((uVar3 & 8) != 0) {
LAB_0006dbde:
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          uVar2 = 0x40000;
          do {
            *(undefined4 *)((int)unaff_A6 + 0x62) = uVar2;
            unaff_A6[0xc] = uVar2;
            candidate_6e2b6();
            *unaff_A6 = 0x6dc18;
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006dcd6;
            uVar3 = candidate_6e230();
            if ((uVar3 & 0x80) != 0) goto LAB_0006ddf2;
            if ((uVar3 & 2) != 0) goto LAB_0006dae2;
            if ((uVar3 & 0x20) != 0) goto LAB_0006dcce;
            if ((uVar3 & 8) != 0) {
              if (*(int *)((int)unaff_A6 + 0x62) < 1) break;
LAB_0006dc68:
              candidate_14594();
              candidate_6e27a();
              candidate_6e390();
              goto LAB_0006deba;
            }
            if ((uVar3 & 4) != 0) {
              if (*(int *)((int)unaff_A6 + 0x62) < 0) goto LAB_0006dc68;
              break;
            }
            candidate_6e2b6();
            candidate_1b1dc();
            *unaff_A6 = 0x6db94;
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006dcd6;
            uVar3 = candidate_6e230();
            if ((uVar3 & 0x80) != 0) goto LAB_0006ddf2;
            if ((uVar3 & 2) != 0) goto LAB_0006dae2;
            if ((uVar3 & 4) == 0) goto code_r0x0006dbba;
LAB_0006dbf2:
            if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) break;
            uVar2 = 0xfffc0000;
          } while( true );
        }
        candidate_6e2b6();
        candidate_1b1dc();
        candidate_6e29c();
        *unaff_A6 = 0x6dc90;
        uVar8 = 0;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006dcd6;
        uVar3 = candidate_6e230();
        if ((uVar3 & 2) != 0) goto LAB_0006dae2;
        if ((uVar3 & 0x20) != 0) goto LAB_0006dcce;
        candidate_14594();
        candidate_6e27a();
        if ((bool)uVar8) {
          *unaff_A6 = candidate_6db80;
        }
        *(undefined2 *)(unaff_A6 + 0x1e) = 0x8080;
        candidate_6e390();
        goto LAB_0006deba;
      }
      if ((uVar3 & 0x20) != 0) goto LAB_0006dcce;
      candidate_14594();
      candidate_6e27a();
      if ((bool)uVar8) {
        *unaff_A6 = candidate_6db80;
      }
      candidate_6e390();
    }
    else {
LAB_0006dae2:
      do {
        candidate_6e2b6();
        candidate_1b1dc();
        *unaff_A6 = 0x6daf6;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006dcd6;
        uVar3 = candidate_6e230();
        if ((uVar3 & 0x80) != 0) goto LAB_0006ddf2;
        if ((uVar3 & 0x20) != 0) {
          if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
            candidate_6e2b6();
            candidate_1b1dc();
            *unaff_A6 = 0x6da04;
            uVar8 = 0;
            candidate_6e258();
            candidate_1945c();
            if ((bool)uVar8) {
              *unaff_A6 = candidate_6df7e;
            }
            if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
              candidate_6e27a();
              goto LAB_0006df76;
            }
          }
          candidate_6e2b6();
          *(byte *)((int)unaff_A6 + 0x82) = *(byte *)((int)unaff_A6 + 0x82) & 0xfe;
          candidate_1b1dc();
          *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
          candidate_353a4();
          *unaff_A6 = 0x6da5c;
          uVar8 = 0;
          candidate_6e258();
          candidate_6e27a();
          candidate_1945c();
          if ((bool)uVar8) {
            *unaff_A6 = candidate_6df7e;
            uVar8 = 0;
          }
          uVar4 = candidate_aa610();
          if ((bool)uVar8) {
            *(undefined2 *)((int)unaff_A6 + 0x72) = uVar4;
            bVar9 = false;
            uVar4 = candidate_aa760();
            if (bVar9) {
              *(undefined2 *)(unaff_A6 + 0x1d) = uVar4;
              candidate_aa83a();
              *unaff_A6 = candidate_6dac0;
            }
          }
          goto LAB_0006df76;
        }
        if ((uVar3 & 2) != 0) {
          candidate_14594();
          candidate_6e27a();
          candidate_6e390();
          goto LAB_0006deba;
        }
        candidate_6e2b6();
        candidate_1b1dc();
        *unaff_A6 = 0x6db40;
        uVar8 = 0;
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006dcd6;
        uVar3 = candidate_6e230();
        if ((uVar3 & 0x80) != 0) goto LAB_0006ddf2;
      } while ((uVar3 & 2) != 0);
      if ((uVar3 & 0x20) != 0) {
LAB_0006dcce:
        unaff_A6[0xd] = 0xaf000;
        goto LAB_0006dcd6;
      }
      candidate_14594();
      candidate_6e27a();
      if ((bool)uVar8) {
        *unaff_A6 = candidate_6db80;
      }
      candidate_6e390();
    }
  }
LAB_0006deba:
  if (*(short *)((int)unaff_A6 + 0x76) != 0) {
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
  if ((bVar1 & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar9 = false;
  candidate_aa760();
  if (!bVar9) {
    *unaff_A6 = candidate_6d9e6;
  }
  bVar9 = false;
  candidate_aa9e4();
  if ((bVar9) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_aa7fc();
    *unaff_A6 = candidate_6d9e6;
  }
  if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
    candidate_49e8();
  }
  if (*(char *)((int)unaff_A6 + 0x7e) != '\0') {
    *(char *)((int)unaff_A6 + 0x7e) = *(char *)((int)unaff_A6 + 0x7e) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x7f) != '\0') {
    *(char *)((int)unaff_A6 + 0x7f) = *(char *)((int)unaff_A6 + 0x7f) + -1;
  }
LAB_0006df76:
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
code_r0x0006dbba:
  if ((uVar3 & 8) == 0) goto code_r0x0006dbc2;
  goto LAB_0006dbde;
code_r0x0006dbc2:
  if ((uVar3 & 0x20) != 0) goto LAB_0006dcce;
  candidate_14594();
  candidate_6e27a();
  candidate_6e390();
  goto LAB_0006deba;
}
