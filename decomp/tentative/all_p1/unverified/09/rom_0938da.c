/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0938da.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_938da(void)

{
  short *psVar1;
  short sVar2;
  short *psVar3;
  ushort *extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_120f8();
  candidate_120f8();
  candidate_4256();
  unaff_A6[0x24] = 0x93882;
  candidate_353c2();
  *(undefined2 *)((int)unaff_A6 + 0x8a) = 0x60;
  *unaff_A6 = 0x9391e;
  psVar1 = (short *)unaff_A6[0x24];
  if (psVar1 != (short *)0x0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    psVar3 = psVar1 + 1;
    if (*psVar1 == -1) {
      psVar3 = (short *)((int)psVar3 + *psVar3 + 2);
    }
    unaff_A6[0x24] = psVar3;
    candidate_9d1e();
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0xff;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    candidate_93846();
  }
  else {
    candidate_ca2ce();
    if ((*extraout_A0 & 0x20) != 0) {
      candidate_aa7fc();
    }
    bVar4 = false;
    candidate_aa760();
    if (!bVar4) {
      *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
    }
    candidate_9380a();
  }
  candidate_92832();
  sVar2 = *(short *)((int)unaff_A6 + 0x8a) + -1;
  *(short *)((int)unaff_A6 + 0x8a) = sVar2;
  if (sVar2 < 0) {
    *unaff_A6 = 0x93afa;
  }
  *(undefined1 *)((int)unaff_A6 + 0x86) = *(undefined1 *)unaff_A6[0x1c];
  *(undefined1 *)((int)unaff_A6 + 0x87) = ((undefined1 *)unaff_A6[0x1c])[1];
  return;
}
