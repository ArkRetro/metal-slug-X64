/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0949c4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_949c4(void)

{
  short *psVar1;
  byte bVar2;
  char cVar3;
  undefined1 extraout_D1b;
  short *psVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 in_CF;

  candidate_949ea();
  candidate_92832();
  if ((bool)in_CF) {
    candidate_aa7fc();
  }
  unaff_A6[0x14] = 0;
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
    psVar4 = psVar1 + 1;
    if (*psVar1 == -1) {
      psVar4 = (short *)((int)psVar4 + *psVar4 + 2);
    }
    unaff_A6[0x24] = psVar4;
    candidate_9d1e();
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar5 = false;
  candidate_aa760();
  if (bVar5) {
    cVar3 = candidate_aaa88();
    if (cVar3 == '\x01') {
      *(undefined1 *)((int)unaff_A6 + 0x7b) = extraout_D1b;
      *(undefined1 *)(unaff_A6 + 0x1f) = 0;
    }
  }
  else {
    *unaff_A6 = candidate_93d40;
  }
  bVar5 = false;
  candidate_aa9e4();
  if ((bVar5) || ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
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
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
  *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
  return;
}
