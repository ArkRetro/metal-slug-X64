/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x089e34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_89e34(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar3;

  candidate_1b2be();
  if ((bool)in_CF) {
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xe5382;
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
  }
  candidate_120f8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8800;
  if (*(char *)((int)unaff_A6 + 0x8b) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  candidate_1b1dc();
  *unaff_A6 = 0x89ea6;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
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
  if (*(char *)(unaff_A6 + 0x23) != '\0') {
    *(byte *)(unaff_A6 + 0x23) =
         *(char *)(unaff_A6 + 0x23) - ((byte)*(undefined2 *)(unaff_A5 + -0x7ff2) & 1);
  }
  func_0x0008a618();
  return;
}
