/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06e56c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6e56c(void)

{
  byte bVar1;
  undefined2 uVar3;
  uint uVar2;
  int iVar4;
  int iVar5;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) & 0xfe;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
  do {
    candidate_6ecbc();
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0x6e5b0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
LAB_0006e69c:
      if (*(short *)(unaff_A6 + 0xc) == 0) {
        candidate_6ecbc();
        candidate_4256();
      }
      else {
        candidate_6ecbc();
        candidate_4256();
      }
      *unaff_A6 = 0x6e6d6;
      unaff_A6[0xd] = (-0x80000 - unaff_A6[0xd] >> 5) + unaff_A6[0xd];
      uVar2 = candidate_6ec6a();
      iVar5 = 0;
      iVar4 = 5;
      if ((uVar2 & 4) != 0) {
        iVar5 = -0x40000;
        iVar4 = 2;
      }
      if ((uVar2 & 8) != 0) {
        iVar5 = 0x40000;
        iVar4 = 2;
      }
      unaff_A6[0xc] = (iVar5 - unaff_A6[0xc] >> iVar4) + unaff_A6[0xc];
      candidate_143ee();
      candidate_6ecb4();
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
        *unaff_A6 = candidate_6e63e;
      }
      candidate_6ed5e();
      goto LAB_0006e88e;
    }
    uVar2 = candidate_6ec6a();
    if ((uVar2 & 0x80) != 0) {
LAB_0006e814:
      candidate_1b1dc();
      candidate_aa7fc();
      candidate_6ecbc();
      candidate_4256();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0x90000;
      *(undefined2 *)((int)unaff_A6 + 0x26) = 0x600;
      *unaff_A6 = 0x6e850;
      uVar6 = 0;
      candidate_6ec92();
      candidate_1945c();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_6e95c;
      }
      candidate_6ecb4();
      candidate_49e8();
      *(undefined2 *)(unaff_A6 + 0x1d) = 0x8000;
LAB_0006e884:
      candidate_13f1e();
      goto LAB_0006e94e;
    }
    if ((uVar2 & 0x20) != 0) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        candidate_6ecbc();
        candidate_4256();
        candidate_1b1dc();
        *unaff_A6 = 0x6e4aa;
        uVar6 = 0;
        candidate_6ec92();
        candidate_1945c();
        if ((bool)uVar6) {
          *unaff_A6 = candidate_6e95c;
        }
        if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
          candidate_6ecb4();
          goto LAB_0006e884;
        }
      }
      candidate_6ecbc();
      candidate_4256();
      candidate_1b1dc();
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      candidate_353a4();
      *unaff_A6 = 0x6e508;
      uVar6 = 0;
      candidate_6ec92();
      candidate_6ecb4();
      candidate_1945c();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_6e95c;
        uVar6 = 0;
      }
      uVar3 = candidate_aa610();
      if ((bool)uVar6) {
        *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar3;
        bVar7 = false;
        uVar3 = candidate_aa760();
        if (bVar7) {
          *(undefined2 *)(unaff_A6 + 0x1c) = uVar3;
          candidate_aa83a();
          *unaff_A6 = candidate_6e56c;
        }
      }
      goto LAB_0006e884;
    }
    if ((uVar2 & 2) != 0) {
      candidate_14594();
      candidate_6ecb4();
      candidate_6ed5e();
      goto LAB_0006e88e;
    }
    candidate_6ecbc();
    candidate_4256();
    candidate_1b1dc();
    *unaff_A6 = 0x6e606;
    uVar6 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) goto LAB_0006e69c;
    uVar2 = candidate_6ec6a();
    if ((uVar2 & 0x80) != 0) goto LAB_0006e814;
    if ((uVar2 & 2) == 0) {
      candidate_14594();
      candidate_6ecb4();
      if ((bool)uVar6) {
        *unaff_A6 = candidate_6e63e;
      }
      candidate_6ed5e();
LAB_0006e88e:
      if (*(short *)((int)unaff_A6 + 0x72) != 0) {
        *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      }
      bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
      *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
      if ((bVar1 & 0x40) != 0) {
        *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) & 0xfe;
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      if ((DAT_0010f000 & 0x80) != 0) {
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
      }
      bVar7 = false;
      candidate_aa760();
      if (!bVar7) {
        *unaff_A6 = candidate_6e480;
      }
      if (*(char *)((int)unaff_A6 + 0x7d) != '\0') {
        *(char *)((int)unaff_A6 + 0x7d) = *(char *)((int)unaff_A6 + 0x7d) + -1;
      }
      if (*(char *)((int)unaff_A6 + 0x7e) != '\0') {
        *(char *)((int)unaff_A6 + 0x7e) = *(char *)((int)unaff_A6 + 0x7e) + -1;
      }
      if ((*(byte *)(unaff_A6 + 0x20) & 1) != 0) {
        *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) & 0xfe;
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
        candidate_aa7fc();
        *unaff_A6 = candidate_6e480;
      }
      candidate_6ecbc();
      candidate_140c0();
LAB_0006e94e:
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(byte *)(unaff_A6 + 0x20) = *(byte *)(unaff_A6 + 0x20) & 0xfe;
      return;
    }
  } while( true );
}
