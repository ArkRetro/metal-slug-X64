/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0128e8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_128e8(void)

{
  char cVar1;
  undefined4 *puVar2;
  int unaff_A5;
  int unaff_A6;

  *(undefined1 *)(unaff_A5 + 0x400a) = 0;
  *(undefined1 *)(unaff_A5 + 0x400d) = 0xff;
  if (unaff_A6 == 0x1014c4) {
LAB_00012918:
    puVar2 = *(undefined4 **)(candidate_42e8 + unaff_A5);
  }
  else {
    if (unaff_A6 != 0x101574) {
      cVar1 = candidate_aab66();
      if (cVar1 < '\0') {
        puVar2 = (undefined4 *)0x16dbe;
        goto LAB_0001292e;
      }
      if (cVar1 == '\0') goto LAB_00012918;
    }
    puVar2 = *(undefined4 **)(thunk_FUN_000044bc + unaff_A5);
  }
LAB_0001292e:
  *(undefined4 *)(unaff_A5 + 0x401c) = *puVar2;
  *(undefined4 *)(unaff_A5 + 0x4020) = puVar2[1];
  return;
}
