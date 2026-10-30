/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e8c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e8c2(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  candidate_19dfc();
  candidate_120f8();
  if ((*(byte *)((int)unaff_A6 + 0x96) & 1) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9e906;
  uVar2 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_9ec34;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
