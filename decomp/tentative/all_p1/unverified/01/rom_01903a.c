/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01903a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined2 candidate_1903a(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int extraout_A0;
  int unaff_A5;
  char cVar3;
  bool bVar4;

  uVar2 = *(undefined2 *)(unaff_A5 + 0x69b8);
  cVar3 = false;
  uVar1 = candidate_1964a();
  if (!(bool)cVar3) {
    return uVar1;
  }
  func_0x000040ac();
  bVar4 = false;
  uVar1 = func_0x000040ac();
  if ((bVar4) && (*(char *)(extraout_A0 + 0x7a) != '\0')) {
    uVar2 = *(undefined2 *)(extraout_A0 + 0x3c);
  }
  if (bVar4 == false) {
    uVar2 = uVar1;
  }
  if (-1 < (char)(-cVar3 >> 7)) {
    uVar1 = uVar2;
  }
  return uVar1;
}
