/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04e962.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_4e962(void)

{
  int in_D0;
  int iVar1;
  int in_D1;
  byte bVar2;

  bVar2 = 0;
  if (in_D0 < 0) {
    bVar2 = 0xff;
  }
  if (in_D1 < 0) {
    bVar2 = ~bVar2;
  }
  iVar1 = candidate_4e648();
  if (bVar2 != 0) {
    iVar1 = -iVar1;
  }
  return iVar1;
}
