/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09085a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9085a(void)

{
  byte bVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar3;

  candidate_91416();
  bVar2 = candidate_919f4();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_90042;
    uVar3 = false;
  }
  else {
    bVar1 = *(byte *)((int)unaff_A6 + 0x92);
    if (bVar2 != bVar1) {
      *unaff_A6 = candidate_90042;
    }
    uVar3 = bVar2 == bVar1 && bVar2 < bVar1;
  }
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_903c4;
    uVar3 = 0;
  }
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
  }
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar3 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
