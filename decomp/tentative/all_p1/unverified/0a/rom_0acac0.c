/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0acac0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_acac0(void)

{
  uint uVar1;
  short sVar2;
  char cVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_aca7c();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  cVar3 = candidate_c05b8();
  if (cVar3 == '\0') {
    candidate_ae244();
  }
  else {
    candidate_ae244();
  }
  *unaff_A6 = 0xacaf6;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    uVar1 = unaff_A6[0xd];
    uVar4 = 0x4fff < uVar1;
    unaff_A6[0xd] = uVar1 - 0x5000;
    candidate_143ee();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar4 = 0;
    candidate_14594();
  }
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_acb3a;
  }
  if (*(char *)((int)unaff_A6 + 0x96) == '\0') {
    bVar5 = false;
    if (*(short *)(unaff_A6 + 0x23) == 0) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
      candidate_ae8c2();
      if (bVar5) {
        *(undefined2 *)(unaff_A6 + 0x23) = 0x3c;
        *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
        *(undefined2 *)((int)unaff_A6 + 0x8e) = 2;
        candidate_120f8();
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      candidate_ae8c2();
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      if (*(short *)((int)unaff_A6 + 0x8e) == 0) {
        sVar2 = *(short *)(unaff_A6 + 0x23) + -1;
        *(short *)(unaff_A6 + 0x23) = sVar2;
        if (sVar2 == 0) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
        }
        if ((*(ushort *)(unaff_A6 + 0x23) & 2) == 0) {
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
        }
        else {
          *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
          *(undefined1 *)(unaff_A6 + 0x21) = 0xff;
        }
      }
      else {
        *(short *)((int)unaff_A6 + 0x8e) = *(short *)((int)unaff_A6 + 0x8e) + -1;
      }
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      candidate_ca2ce();
      if (*(char *)((int)unaff_A6 + 0x9d) == '\0') {
        candidate_ae610();
      }
      candidate_ae91a();
    }
    bVar5 = false;
    candidate_aa760();
    if (!bVar5) {
      *unaff_A6 = candidate_ac870;
    }
    candidate_140c0();
  }
  cVar3 = *(char *)(unaff_A6 + 0x26);
  *(char *)(unaff_A6 + 0x26) = cVar3 + -1;
  if (cVar3 == '\0') {
    *(undefined1 *)(unaff_A6 + 0x26) = 0;
  }
  return;
}
