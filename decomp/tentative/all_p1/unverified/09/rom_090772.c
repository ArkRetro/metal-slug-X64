/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x090772.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_90772(void)

{
  bool bVar1;
  byte bVar2;
  byte bVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_914a2();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  unaff_A6[0x14] = 0x91cec;
  candidate_4e76();
  bVar3 = candidate_260c8();
  *(byte *)(unaff_A6 + 0x23) = bVar3 >> 1;
  candidate_4256();
  *unaff_A6 = 0x907ba;
  uVar4 = 0;
  candidate_91416();
  candidate_919f4();
  if (!(bool)uVar4) {
    *unaff_A6 = candidate_90042;
    uVar4 = 0;
  }
  candidate_427a();
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  bVar3 = *(byte *)(unaff_A6 + 0x23);
  bVar2 = bVar3 - 1;
  *(byte *)(unaff_A6 + 0x23) = bVar2;
  bVar1 = SBORROW1(bVar3,'\x01') == (short)((ushort)bVar2 << 8) < 0;
  if (bVar2 == 0 || !bVar1) {
    *unaff_A6 = 0x90698;
  }
  uVar4 = (bVar2 != 0 && bVar1) && bVar3 == 0;
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar4 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
