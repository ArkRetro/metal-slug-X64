/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e8a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e8a8(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 in_CF;

  candidate_1436a();
  candidate_427a();
  if ((bool)in_CF) {
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
