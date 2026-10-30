/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0946fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_946fa(void)

{
  short *psVar1;
  byte bVar2;
  ushort uVar3;
  undefined2 uVar4;
  char cVar5;
  undefined1 extraout_D1b;
  int iVar6;
  int iVar7;
  ushort *extraout_A0;
  short *psVar8;
  ushort *extraout_A0_00;
  undefined4 *unaff_A6;
  undefined1 uVar9;
  bool bVar10;

  if (*(short *)((int)unaff_A6 + 0x4e) == 0) {
    candidate_120f8();
    candidate_120f8();
    candidate_4256();
    unaff_A6[0x24] = 0x94a62;
    candidate_353c2();
    *(undefined2 *)((int)unaff_A6 + 0x8a) = 0x60;
    *unaff_A6 = 0x94afe;
    psVar1 = (short *)unaff_A6[0x24];
    if (psVar1 != (short *)0x0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      psVar8 = psVar1 + 1;
      if (*psVar1 == -1) {
        psVar8 = (short *)((int)psVar8 + *psVar8 + 2);
      }
      unaff_A6[0x24] = psVar8;
      candidate_9d1e();
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x7e) = 0xff;
    if (*(short *)(unaff_A6 + 0x20) < 0) {
      candidate_94a26();
    }
    else {
      candidate_ca2ce();
      if ((*extraout_A0_00 & 0x20) != 0) {
        candidate_aa7fc();
      }
      bVar10 = false;
      candidate_aa760();
      if (!bVar10) {
        *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
      }
      candidate_949ea();
    }
    candidate_92832();
    uVar3 = *(ushort *)((int)unaff_A6 + 0x8a) - 1;
    *(ushort *)((int)unaff_A6 + 0x8a) = uVar3;
    if ((int)((uint)uVar3 << 0x10) < 0) {
      *unaff_A6 = 0x94cda;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
        candidate_120f8();
      }
      else {
        candidate_120f8();
      }
      candidate_4256();
      unaff_A6[0xd] = unaff_A6[0xd] + 0x7c32f;
      *unaff_A6 = 0x947cc;
      *(byte *)((int)unaff_A6 + 0x7e) = *(byte *)((int)unaff_A6 + 0x7e) | 0x3e;
      unaff_A6[0xd] = (-0x5c28c0 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
      candidate_ca2ce();
      iVar7 = 0;
      iVar6 = 6;
      if ((*extraout_A0 & 4) != 0) {
        iVar7 = -0x40000;
        iVar6 = 2;
      }
      if ((*extraout_A0 & 8) != 0) {
        iVar7 = 0x40000;
        iVar6 = 2;
      }
      unaff_A6[0xc] = (iVar7 - unaff_A6[0xc] >> iVar6) + unaff_A6[0xc];
      uVar4 = candidate_14c0c();
      *(undefined2 *)(unaff_A6 + 0x21) = uVar4;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        candidate_4256();
        *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
        *unaff_A6 = 0x94756;
      }
      candidate_92832();
    }
    else {
      if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
        candidate_120f8();
      }
      else {
        candidate_120f8();
      }
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      *unaff_A6 = 0x94756;
      uVar9 = 0;
      *(byte *)((int)unaff_A6 + 0x7e) = *(byte *)((int)unaff_A6 + 0x7e) | 0x3e;
      candidate_949ea();
      candidate_92832();
      if ((bool)uVar9) {
        unaff_A6[0x24] = 0;
        *unaff_A6 = candidate_93e7a;
        if (*(short *)((int)unaff_A6 + 0x4e) == 0) {
          *unaff_A6 = candidate_94aba;
        }
      }
    }
    if (*(short *)((int)unaff_A6 + 0x8a) != 0) {
      *(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1;
    }
    bVar2 = *(byte *)((int)unaff_A6 + 0x5f);
    *(byte *)((int)unaff_A6 + 0x5f) = bVar2 & 0xbf;
    if (((bVar2 & 0x40) != 0) && (*(char *)((int)unaff_A6 + 0x5a) != '\x0f')) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    psVar1 = (short *)unaff_A6[0x24];
    if (psVar1 == (short *)0x0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
      if (*(char *)((int)unaff_A6 + 0x7e) != '\0') {
        bVar2 = *(char *)((int)unaff_A6 + 0x7e) - 1;
        *(byte *)((int)unaff_A6 + 0x7e) = bVar2;
        if ((bVar2 & 1) != 0) {
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
          *(undefined1 *)(unaff_A6 + 0x22) = 0xff;
        }
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
      psVar8 = psVar1 + 1;
      if (*psVar1 == -1) {
        psVar8 = (short *)((int)psVar8 + *psVar8 + 2);
      }
      unaff_A6[0x24] = psVar8;
      candidate_9d1e();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    if ((DAT_0010f000 & 0x80) != 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    }
    bVar10 = false;
    candidate_aa760();
    if (bVar10) {
      cVar5 = candidate_aaa88();
      if (cVar5 == '\x01') {
        *(undefined1 *)((int)unaff_A6 + 0x7b) = extraout_D1b;
        *(undefined1 *)(unaff_A6 + 0x1f) = 0;
      }
    }
    else {
      *unaff_A6 = candidate_93d40;
    }
    bVar10 = false;
    candidate_aa9e4();
    if ((bVar10) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      if (*(char *)((int)unaff_A6 + 0x5a) == '\x0f') {
        *unaff_A6 = 0x94cda;
      }
      else {
        *unaff_A6 = candidate_946fa;
      }
    }
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && (*(int *)((int)unaff_A6 + 0x62) != 0)) {
      candidate_49e8();
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
  *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
  return;
}
