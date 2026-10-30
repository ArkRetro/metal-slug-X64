/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086836.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86836(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_1436a();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_86850;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
