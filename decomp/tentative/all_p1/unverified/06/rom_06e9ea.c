/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06e9ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6e9ea(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  *unaff_A6 = 0x6e9fa;
  bVar4 = false;
  candidate_409a();
  if (bVar4) {
    cVar3 = candidate_aab84();
    if (-1 < cVar3) {
      *unaff_A6 = candidate_6ea18;
    }
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
