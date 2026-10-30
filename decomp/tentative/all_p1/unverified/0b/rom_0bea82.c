/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bea82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bea82(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  bVar3 = *(char *)((int)unaff_A6 + 0x6e) + 1;
  *(byte *)((int)unaff_A6 + 0x6e) = bVar3;
  if (0x80 < bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  func_0x00009cce();
  return;
}
