/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b79c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b79c(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe06f;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe06f;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x9f) = 0;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x4b7ec;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = *(undefined1 *)(unaff_A6[3] + 0x9a);
  candidate_1abc8();
  candidate_4d9de();
  if ((*(char *)((int)unaff_A6 + 0x61) == '\0') && (*(char *)(unaff_A6 + 0x18) != '\0')) {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 0xff;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  if (*(char *)((int)unaff_A6 + 0x9f) != '\0') {
    candidate_40bc();
    *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x98);
    candidate_1acf2();
  }
  if ((*(byte *)(unaff_A6[3] + 0x61) & 1) != 0) {
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
