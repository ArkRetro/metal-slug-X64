/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05de22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5de22(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  *unaff_A6 = 0x5de4c;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
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
  candidate_1abc8();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(unaff_A6[3] + 0x1e) + -1;
  if (*(char *)(unaff_A6[3] + 0x86) == '\0') {
    *unaff_A6 = 0x5ddca;
  }
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x86) == '\x03') {
    *unaff_A6 = candidate_5de84;
    return;
  }
  if (*(char *)(unaff_A6[3] + 0x86) == '\x02') {
    *unaff_A6 = candidate_5de98;
    return;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)(extraout_A0 + 0x5a) = *(undefined1 *)((int)unaff_A6 + 0x5a);
    *(byte *)(unaff_A6[3] + 0x5f) = *(byte *)(unaff_A6[3] + 0x5f) | 0x80;
    *unaff_A6 = candidate_5de98;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
