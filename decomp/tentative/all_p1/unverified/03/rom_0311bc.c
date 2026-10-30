/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0311bc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_311bc(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  *unaff_A6 = 0x311c2;
  candidate_427a();
  bVar3 = *(byte *)((int)unaff_A6 + 0x12);
  *(byte *)((int)unaff_A6 + 0x12) = bVar3 - 0x18;
  if (bVar3 < 0x18) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
