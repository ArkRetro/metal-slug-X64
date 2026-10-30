/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09fdfe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9fdfe(void)

{
  byte *pbVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  uVar2 = 0;
  candidate_1ad6e();
  candidate_1af9a();
  if (!(bool)uVar2) {
    if (*(ushort *)(extraout_A0 + 0x3c) < *(ushort *)(unaff_A6 + 0xf)) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      pbVar1 = (byte *)(unaff_A6[3] + 0x90);
      *pbVar1 = *pbVar1 & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      pbVar1 = (byte *)(unaff_A6[3] + 0x90);
      *pbVar1 = *pbVar1 | 1;
    }
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_9ea8();
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  candidate_4256();
  *unaff_A6 = 0x9feaa;
  candidate_a07d0();
  uVar2 = false;
  if (-1 < *(char *)((int)unaff_A6 + 0x8b)) {
    candidate_427a();
    if ((bool)uVar2) {
      *(undefined1 *)(unaff_A6[3] + 0x61) = 0xff;
      uVar2 = 0;
    }
    candidate_4c24();
    if (((bool)uVar2) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_9fd60;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(byte *)(unaff_A6[3] + 0x5f) = *(byte *)(unaff_A6[3] + 0x5f) | 0x80;
    if ((*(byte *)(unaff_A6 + 0x24) & 0x20) == 0) {
      candidate_9f562();
    }
    else {
      candidate_9f58c();
    }
  }
  return;
}
