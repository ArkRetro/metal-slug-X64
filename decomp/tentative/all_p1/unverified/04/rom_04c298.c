/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c298.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c298(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ac38();
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    candidate_4192();
    *unaff_A6 = candidate_1c6ee;
    candidate_120f8();
  }
  if (*(char *)(unaff_A6[3] + 0x61) != '\0') {
    candidate_4192();
    *unaff_A6 = candidate_1c6ee;
  }
  bVar4 = false;
  candidate_4e092();
  if (bVar4) {
    candidate_4192();
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
