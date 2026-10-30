/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0438ae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_438ae(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_438c8;
    in_CF = 0;
  }
  candidate_1945c();
  if (!(bool)in_CF) {
    candidate_4c24();
    if ((bool)in_CF) {
      *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | 6;
      if (*(char *)((int)unaff_A6 + 0x5a) == '\f') {
        *unaff_A6 = candidate_43f32;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        *unaff_A6 = candidate_43f32;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if (*(char *)(unaff_A6 + 0x27) != '\0') {
      *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
    }
    candidate_446f0();
    return;
  }
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
