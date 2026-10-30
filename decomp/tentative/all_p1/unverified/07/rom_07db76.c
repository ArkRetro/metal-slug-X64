/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07db76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7db76(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1abc8();
  candidate_427a();
  bVar1 = *(byte *)(unaff_A6[3] + 0x60);
  if (bVar1 == 0x80) {
    *unaff_A6 = candidate_7dbb6;
  }
  uVar2 = bVar1 < 0x80;
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_7dc0c;
    uVar2 = 0;
  }
  candidate_7dcbc();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_7dc7a;
  }
  return;
}
