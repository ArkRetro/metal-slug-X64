/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fd48.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5fd48(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  if ((*(char *)((int)unaff_A6 + 0x89) == '\0') && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0))
  {
    if (*(char *)((int)unaff_A6 + 0x5a) == '\r') {
      *unaff_A6 = candidate_5f8d4;
    }
    else {
      *unaff_A6 = candidate_5f904;
    }
  }
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
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
