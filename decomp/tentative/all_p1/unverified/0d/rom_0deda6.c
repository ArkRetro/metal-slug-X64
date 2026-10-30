/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0deda6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_deda6(void)

{
  int in_D0;
  int iVar1;
  short in_D1w;
  byte bVar2;

  bVar2 = -(in_D0 < 0);
  if ((in_D1w != 0) && (in_D1w < 0)) {
    bVar2 = ~bVar2;
  }
  iVar1 = candidate_ded8a();
  if (bVar2 != 0) {
    iVar1 = -iVar1;
  }
  return iVar1;
}
