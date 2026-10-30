/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03541a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3541a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_146e8();
  candidate_1436a();
  candidate_3c520();
  *unaff_A6 = 0x35432;
  candidate_1436a();
  if (*(char *)(unaff_A6[3] + 0x8d) == '\0') {
    *(undefined1 *)(unaff_A6[3] + 0x8e) = 0;
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
